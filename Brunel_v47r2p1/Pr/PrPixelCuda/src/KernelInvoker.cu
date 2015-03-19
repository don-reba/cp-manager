#include "Kernel.cuh"
#include "KernelInvoker.cuh"
#include "KernelPreprocessing.h"

#include <boost/timer/timer.hpp>

#include <iostream>
#include <utility>
#include <vector>

cudaError_t invokeParallelSearch(
    dim3                         numThreads,
    const std::vector<uint8_t> & input,
    std::vector<uint8_t>       & solution,
    std::ostream               & logger) {
  boost::timer::auto_cpu_timer t;

  // For now, just perform what we did before
  // (backwards compatibility)
  logger << "Input pointer: "
    << std::hex << "0x" << (long long int) &(input[0])
    << std::dec << std::endl;

  Event event;
  parseEvent(const_cast<uint8_t*>(&input[0]), input.size(), event);
  printInfo(event, logger);

  char  *dev_input             = 0;
  int   *dev_num_tracks        = 0;
  int   *dev_track_indexes     = 0;
  Track *dev_tracks            = 0;
  bool  *dev_track_holders     = 0;
  int   *dev_prevs             = 0;
  int   *dev_nexts             = 0;
  int   *dev_tracks_to_process = 0;
  cudaError_t cudaStatus = cudaSuccess;

  // Choose which GPU to run on, change this on a multi-GPU system.
  cudaCheck(cudaSetDevice(0));

  // Allocate memory
  // Allocate CPU buffers
  Track *tracks          = (Track*) malloc(MAX_TRACKS * sizeof(Track));
  int   *num_tracks      = (int*)   malloc(sizeof(int));
  int   *h_prevs         = (int*)   malloc(event.no_hits[0] * sizeof(int));
  int   *h_nexts         = (int*)   malloc(event.no_hits[0] * sizeof(int));
  bool  *h_track_holders = (bool*)  malloc(MAX_TRACKS * sizeof(bool));
  int   *track_indices   = (int*)   malloc(MAX_TRACKS * sizeof(int));

  // Allocate GPU buffers
  cudaCheck(cudaMalloc((void**)&dev_tracks,            MAX_TRACKS * sizeof(Track)));
  cudaCheck(cudaMalloc((void**)&dev_track_holders,     MAX_TRACKS * sizeof(bool)));
  cudaCheck(cudaMalloc((void**)&dev_track_indexes,     MAX_TRACKS * sizeof(int)));
  cudaCheck(cudaMalloc((void**)&dev_tracks_to_process, MAX_TRACKS * sizeof(int)));

  cudaCheck(cudaMalloc((void**)&dev_prevs, event.no_hits[0] * sizeof(int)));
  cudaCheck(cudaMalloc((void**)&dev_nexts, event.no_hits[0] * sizeof(int)));

  // Copy input file from host memory to GPU buffers
  cudaCheck(cudaMalloc((void**)&dev_input,      input.size()));
  cudaCheck(cudaMalloc((void**)&dev_num_tracks, sizeof(int)));

  // memcpys
  cudaCheck(cudaMemcpy(dev_input, input.data(), input.size(), cudaMemcpyHostToDevice));

  // clear track_holders flags
  cudaCheck(cudaMemset(dev_track_holders, 0, MAX_TRACKS * sizeof(bool)));

  // Launch a kernel on the GPU with one thread for each element.
  prepareData<<<1, 1>>>(dev_input, dev_prevs, dev_nexts);

  //----------------------

  const int threadCount         = 32;
  const int candidatesPerThread = 100000;
  std::vector<Span> spans;
  splitHits(event, threadCount, candidatesPerThread, spans);

  Span * devSpans;
  const int devSpansSize = spans.size() * sizeof(Span);
  cudaCheck(cudaMalloc((void**)&devSpans, devSpansSize));
  cudaCheck(cudaMemcpy(devSpans, spans.data(), devSpansSize, cudaMemcpyHostToDevice));

  const int numBlocks = spans.size() / threadCount;

  Fit * devFittings;
  cudaCheck(cudaMalloc((void**)&devFittings, spans.size() * sizeof(Fit)));

  gpuKalmanBalanced<<<numBlocks, threadCount>>>(dev_tracks, devSpans, devFittings);
  cudaCheckLast("gpuKalmanBalanced");

  consolidateHits<<<1, 1>>>(devFittings, spans.size(), dev_tracks, dev_track_holders);
  cudaCheckLast("consolidateHits");

  // Launch a kernel on the GPU with one thread for each element.
  //prepareData<<<1, 1>>>(dev_input, dev_prevs, dev_nexts, dev_track_holders);

  // gpuKalman
  logger << "gpuKalman" << std::endl;
  //cudaEvent_t start_kalman, start_postprocess, stop;
  //float t0, t1, t2;

  //cudaEventCreate(&start_kalman);
  //cudaEventCreate(&start_postprocess);
  //cudaEventCreate(&stop);

  //cudaEventRecord(start_kalman, 0);

  // 4 of the sensors are unused, because the algorithm needs 5-sensor spans
  //const int effective_no_sensors = *event.no_sensors - 4;
  //gpuKalman<<<effective_no_sensors, numThreads>>>(dev_tracks, dev_track_holders);

  //cudaEventRecord(start_postprocess);

  logger << "postProcess" << std::endl;
  postProcess<<<1, numThreads>>>(dev_tracks, dev_track_holders, dev_track_indexes, dev_num_tracks, dev_tracks_to_process);

  cudaCheck(cudaDeviceSynchronize());

  //cudaEventRecord(stop, 0);
  //cudaEventSynchronize(stop);

  //cudaEventElapsedTime(&t0, start_kalman, start_postprocess);
  //cudaEventElapsedTime(&t1, start_postprocess, stop);
  //cudaEventElapsedTime(&t2, start_kalman, stop);
  //cudaEventDestroy(start_kalman);
  //cudaEventDestroy(start_postprocess);
  //cudaEventDestroy(stop);

  // get the results
  cudaCheck(cudaMemcpy(h_track_holders, dev_track_holders, MAX_TRACKS * sizeof(bool), cudaMemcpyDeviceToHost));
  cudaCheck(cudaMemcpy(track_indices, dev_track_indexes, MAX_TRACKS * sizeof(int), cudaMemcpyDeviceToHost));
  cudaCheck(cudaMemcpy(tracks, dev_tracks, MAX_TRACKS * sizeof(Track), cudaMemcpyDeviceToHost));
  cudaCheck(cudaMemcpy(num_tracks, dev_num_tracks, sizeof(int), cudaMemcpyDeviceToHost));

  // number of tracks after stage#1
  int no_tracks_stage1 = 0;
  for (int i = 0; i < event.no_hits[0]; ++i) {
    if (h_track_holders[i])
      ++no_tracks_stage1;
  }

  // copy selected track to the solution vector
  if (*num_tracks > 0) {
    solution.resize(*num_tracks * sizeof(Track));
    Track * solutionTracks = (Track*)&solution[0];
    for (size_t i = 0; i != *num_tracks; ++i)
      solutionTracks[i] = tracks[track_indices[i]];
  }

  // print debug info
  for (int i = 0; i < num_tracks[0]; ++i)
    printTrack(tracks, track_indices[i], event, logger);
  logger << "Processed " << num_tracks[0] << " tracks" << std::endl;

  free(h_prevs);
  free(h_nexts);
  free(h_track_holders);
  free(tracks);
  free(num_tracks);

  cudaCheck(cudaFree(devSpans));
  cudaCheck(cudaFree(devFittings));

  return cudaStatus;
}

// #track, h0, h1, h2, h3, ..., hn, length, chi2
void printTrack(Track* tracks, int track_no, const Event & event, std::ostream& logger) {
  logger << track_no << ": ";

  Track t = tracks[track_no];
  for (int i = 0; i < t.hitsNum; ++i)
    logger << event.hit_IDs[t.hits[i]] << ", ";

  logger << "length: " << (int) t.hitsNum << std::endl;
}

void printOutAllSensorHits(int* prevs, int* nexts, const Event & event, std::ostream& logger) {
  logger << "All valid sensor hits: " << std::endl;
  for (int i = 0; i < *event.no_sensors; ++i) {
    for (int j = 0; j < event.sensor_hitNums[i]; ++j) {
      int hit = event.sensor_hitStarts[i] + j;

      if (nexts[hit] != -1)
        logger << hit << ", " << nexts[hit] << std::endl;
    }
  }
}

void printOutSensorHits(int sensorNumber, int* prevs, int* nexts, const Event & event, std::ostream& logger) {
  for (int i = 0; i < event.sensor_hitNums[sensorNumber]; ++i) {
    int hstart = event.sensor_hitStarts[sensorNumber];

    logger << hstart + i << ": " << prevs[hstart + i] << ", " << nexts[hstart + i] << std::endl;
  }
}

void printInfo(const Event & event, std::ostream& logger) {
  logger << "Read info:" << std::endl
    << " no sensors: " << event.no_sensors << std::endl
    << " no hits: " << event.no_hits << std::endl
    << "First 5 sensors: " << std::endl;

  for (int i = 0; i < 5; ++i) {
    logger << " Zs: " << event.sensor_Zs[i] << std::endl
      << " hitStarts: " << event.sensor_hitStarts[i] << std::endl
      << " hitNums: " << event.sensor_hitNums[i] << std::endl << std::endl;
  }

  logger << "First 5 hits: " << std::endl;

  for (int i = 0; i < 5; ++i) {
    logger << " hit_id: " << event.hit_IDs[i] << std::endl;
    logger << " hit_X:  " << event.hit_Xs[i]  << std::endl;
    logger << " hit_Y:  " << event.hit_Ys[i]  << std::endl;
    logger << " hit_Z:  " << event.hit_Zs[i]  << std::endl;
    logger << std::endl;
  }
}

void getMaxNumberOfHits(char*& input, int& maxHits) {
  int * l_no_sensors       = (int*) input;
  int * l_no_hits          = (int*) (l_no_sensors + 1);
  int * l_sensor_Zs        = (int*) (l_no_hits + 1);
  int * l_sensor_hitStarts = (int*) (l_sensor_Zs + l_no_sensors[0]);
  int * l_sensor_hitNums   = (int*) (l_sensor_hitStarts + l_no_sensors[0]);

  maxHits = 0;
  for (int i = 0; i < l_no_sensors[0]; ++i) {
    if (l_sensor_hitNums[i] > maxHits)
      maxHits = l_sensor_hitNums[i];
  }
}
