#include <GpuHandler/IGpuHandler.h>

#include "Definitions.cuh"
#include "kernel.cuh"

#include "Tools.h"
#include "kernelInvoker.cuh"

// DBG
#include <iostream>

#pragma GCC visibility push(default)
class PrPixelCudaHandler : IGpuHandler
{
  public:
    PrPixelCudaHandler()
    {
      std::cout << "In PrPixelCudaHandler::PrPixelCudaHandler!" << std::endl;
    }

    virtual void operator() ()
    {
      std::cout << "In PrPixelCudaHandler::operator()!" << std::endl;
    }
};
#pragma GCC visiblity pop

// TODO: debug purposes only
extern float* h_hit_Xs;
extern float* h_hit_Ys;
extern int*   h_hit_Zs;

extern int*   h_prevs;
extern int*   h_nexts;

// TODO: Add debug machinery

template<class T>
void combineWithElement(std::vector<char>& combined_vector, T element){
    char* t = (char*) &(element);
    for(int i=0; i<sizeof(element); i++)
        combined_vector.push_back(t[i]);
}

template<class T>
void combineWithVector(std::vector<char>& combined_vector, std::vector<T> input){
    for (typename std::vector<T>::iterator it = input.begin(); it< input.end(); it++){
        combineWithElement(combined_vector, (*it));
    }
}

void printToFile(int num_tracks, int* track_indexes, Track* tracks, int event_no);

void buildAndCombine(std::vector<char>& combined_vector, const PixelEvent& event){

    // combine all
    // TODO: Make this use malloc and memcpy (much more efficient) 

    // Sizes of event.sensor* and event.hit*
    combineWithElement(combined_vector, event.noSensors);
    combineWithElement(combined_vector, event.noHits);

    // Data itself
    combineWithVector(combined_vector, event.sensorZs);
    combineWithVector(combined_vector, event.sensorHitStarts);
    combineWithVector(combined_vector, event.sensorHitsNums);
    combineWithVector(combined_vector, event.hitIDs);
    combineWithVector(combined_vector, event.hitXs);
    combineWithVector(combined_vector, event.hitYs);
    combineWithVector(combined_vector, event.hitZs);
}

extern "C" void pixel_tracker_implementation(const PixelEvent * data, GpuTrack ** results, int * result_count) {
  std::vector<char> combined_vector;

  // Serialize the data
  // std::cout << "buildAndCombine" << std::endl;
  buildAndCombine(combined_vector, *data);
  char* combined_vector_pointer = (char*) &combined_vector[0];

  // set h_ pointers to comply with main
  // std::cout << "setpointersfrominput" << std::endl;
  setHPointersFromPixelEvent(*data);
  // setHPointersFromInput(combined_vector_pointer);

  // Write out the input
  std::ofstream outfile("dump_current_event.dump", std::ofstream::binary);
  outfile.write(combined_vector_pointer, combined_vector.size());
  outfile.close();

  // quicksort
  // std::cout << "quicksort" << std::endl;
  // quickSortInput(combined_vector_pointer);

  // Invoke kernel
  Track* tracks;
  int* num_tracks;
  int* track_indexes;
  dim3 numBlocks(48), numThreads(32);

  // std::cout << "invoking kernel" << std::endl;
  cudaError_t cudaStatus = invokeParallelSearch(numBlocks, numThreads, combined_vector_pointer, combined_vector.size(), tracks, num_tracks, track_indexes);
  if (cudaStatus != cudaSuccess) {
    std::cerr << "cuda kernel failed" << std::endl;
  }

  cudaStatus = cudaDeviceReset();
  if (cudaStatus != cudaSuccess) {
    std::cerr << "cudaDeviceReset failed" << std::endl;
  }

  // std::cout << "kernel was succesfully called!" << std::endl;

  // Conversion back to GpuTrack (momentary)
  *result_count = num_tracks[0];
  *results = new GpuTrack[*result_count];
  GpuTrack t;
  Track* h_t;
  for(int i=0; i < num_tracks[0]; ++i){
    GpuTrack & t   = (*results)[i];
    Track    & h_t = tracks[track_indexes[i]];

    t.x0 = h_t.x0;
    t.tx = h_t.tx;
    t.y0 = h_t.y0;
    t.ty = h_t.ty;

    t.s0 = h_t.s0;
    t.sx = h_t.sx;
    t.sz = h_t.sz;
    t.sxz= h_t.sxz;
    t.sz2 = h_t.sz2;

    t.u0 = h_t.u0;
    t.uy = h_t.uy;
    t.uz = h_t.uz;
    t.uyz = h_t.uyz;
    t.uz2 = h_t.uz2;

    t.trackHitsNum = h_t.hitsNum;
    t.hits.clear();
    for(int j=0; j<t.trackHitsNum; ++j)
      t.hits.push_back(h_t.hits[j]);
  }

  free(tracks);
  free(num_tracks);
  free(track_indexes);

  // printToFile(num_tracks[0], track_indexes, tracks, 1);
}

void printToFile(int num_tracks, int* track_indexes, Track* tracks, int event_no){
	std::ofstream fout;
	std::cout << "Printing..." << std::endl;
	fout.open("results/output.out", std::ofstream::app);
	Track current_track;
        fout << "Num tracks: " << num_tracks << std::endl;
	for(int i=0; i<num_tracks; ++i){
		current_track = tracks[track_indexes[i]];
		fout << "Track " << i << ":" << std::endl
		     << " " << current_track.hitsNum << " hits" << std::endl;
		for(int j=0; j<current_track.hitsNum; ++j){
			std::cout << current_track.hits[j] << ", ";
			fout << current_track.hits[j] << ", ";
		}
		fout << std::endl << std::endl;
	}
	fout.close();
}

int main()
{
  // XXX
  delete new PrPixelCudaHandler();

	// Read file (s)
	char* input;
	int size;
	std::string c = "pixel-sft-event-0.dump";
	readFile(c.c_str(), input, size);
	printInfo();

	// Return elements
	Track* tracks;
	int* num_tracks;
	int* track_indexes;

	dim3 numBlocks(48), numThreads(32);

	// Pre-processing, quick sort over X
	// TODO: Why? - Check why changing this causes different results - Even crashing!?
	// quickSortInput(input);

    cudaError_t cudaStatus = invokeParallelSearch(numBlocks, numThreads, input, size, tracks, num_tracks, track_indexes);
    if (cudaStatus != cudaSuccess) {
        std::cerr << "cuda kernel failed" << std::endl;
        return cudaStatus;
    }

    cudaStatus = cudaDeviceReset();
    if (cudaStatus != cudaSuccess) {
        std::cerr << "cudaDeviceReset failed" << std::endl;
        return cudaStatus;
    }

	// std::cout << "Everything went quite well!" << std::endl;

	/*
	VELOModel v;
	osgViewer::Viewer viewer;
	initializeModel(std::string("pixel-sft-event-0.dump"), v, viewer);
	// Add tracks to the model
	for(int i=0; i<h_no_hits[0]; ++i){
		int phit = h_prevs[i];
		if(phit >= 0)
			v.addTrack(osg::Vec3f(h_hit_Xs[phit], h_hit_Ys[phit], (float) h_hit_Zs[phit]),
					   osg::Vec3f(h_hit_Xs[i], h_hit_Ys[i], (float) h_hit_Zs[i]));
	}
	while(1)
		viewer.run();
	*/

	// getchar();

    return 0;
}
