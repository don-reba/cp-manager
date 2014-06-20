#include "Definitions.cuh"
#include "kernel.cuh"
#include "PixelImplementation.h"

#include "Tools.h"
#include "kernelInvoker.cuh"

// DBG
#include <iostream>

#include <vector>

using namespace std;

// TODO: debug purposes only
extern float* h_hit_Xs;
extern float* h_hit_Ys;
extern int*   h_hit_Zs;

extern int*   h_prevs;
extern int*   h_nexts;

// TODO: Add debug machinery

template<class T>
void combineWithElement(vector<char>& combined_vector, T element){
    char* t = (char*) &(element);
    for(int i=0; i<sizeof(element); i++)
        combined_vector.push_back(t[i]);
}

template<class T>
void combineWithVector(vector<char>& combined_vector, vector<T> input){
    for (typename vector<T>::iterator it = input.begin(); it< input.end(); it++){
        combineWithElement(combined_vector, (*it));
    }
}

void printToFile(int num_tracks, int* track_indexes, Track* tracks, int event_no);

void buildAndCombine(vector<char>& combined_vector, const PixelEvent& event){

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

void pixel_tracker_implementation(
    const PixelEvent & event,
    vector<GpuTrack> & result) {
  vector<char> combined_vector;

  // Serialize the data
  // cout << "buildAndCombine" << endl;
  buildAndCombine(combined_vector, event);
  char* combined_vector_pointer = (char*) &combined_vector[0];

  // set h_ pointers to comply with main
  // cout << "setpointersfrominput" << endl;
  setHPointersFromPixelEvent(event);
  // setHPointersFromInput(combined_vector_pointer);

  // Write out the input
  ofstream outfile("dump_current_event.dump", ofstream::binary);
  outfile.write(combined_vector_pointer, combined_vector.size());
  outfile.close();

  // quicksort
  // cout << "quicksort" << endl;
  // quickSortInput(combined_vector_pointer);

  // Invoke kernel
  Track* tracks;
  int* num_tracks;
  int* track_indexes;
  dim3 numBlocks(48), numThreads(32);

  // cout << "invoking kernel" << endl;
  cudaError_t cudaStatus = invokeParallelSearch(
      numBlocks, numThreads, combined_vector_pointer,
      combined_vector.size(), tracks, num_tracks, track_indexes);
  if (cudaStatus != cudaSuccess)
    cerr << "cuda kernel failed" << endl;

  cudaStatus = cudaDeviceReset();
  if (cudaStatus != cudaSuccess)
    cerr << "cudaDeviceReset failed" << endl;

  // cout << "kernel was succesfully called!" << endl;

  // Conversion back to GpuTrack (momentary)
  result.resize(num_tracks[0]);
  GpuTrack t;
  Track* h_t;
  for (size_t i = 0, size = result.size(); i != size; ++i) {
    GpuTrack & t   = result[i];
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
	ofstream fout;
	cout << "Printing..." << endl;
	fout.open("results/output.out", ofstream::app);
	Track current_track;
        fout << "Num tracks: " << num_tracks << endl;
	for(int i=0; i<num_tracks; ++i){
		current_track = tracks[track_indexes[i]];
		fout << "Track " << i << ":" << endl
		     << " " << current_track.hitsNum << " hits" << endl;
		for(int j=0; j<current_track.hitsNum; ++j){
			cout << current_track.hits[j] << ", ";
			fout << current_track.hits[j] << ", ";
		}
		fout << endl << endl;
	}
	fout.close();
}

int main()
{
	// Read file (s)
	char* input;
	int size;
	string c = "pixel-sft-event-0.dump";
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
        cerr << "cuda kernel failed" << endl;
        return cudaStatus;
    }

    cudaStatus = cudaDeviceReset();
    if (cudaStatus != cudaSuccess) {
        cerr << "cudaDeviceReset failed" << endl;
        return cudaStatus;
    }

	// cout << "Everything went quite well!" << endl;

	/*
	VELOModel v;
	osgViewer::Viewer viewer;
	initializeModel(string("pixel-sft-event-0.dump"), v, viewer);
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
