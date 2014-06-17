
#include <iostream>
#include "Definitions.cuh"
// #include "Histo.h"

#include <fstream>
#include <string>
#include <sstream>

extern int* h_no_hits;

void getMaxNumberOfHits(char*& input, int& maxHits);
cudaError_t invokeParallelSearch(dim3 numBlocks, dim3 numThreads,
	char* input, int size, Track*& tracks, int*& num_tracks, int*& track_indexes);

void printTrack(Track* tracks, int i);
void printOutSensorHits(int sensorNumber, int* prevs, int* nexts);
void printOutAllSensorHits(int* prevs, int* nexts);

template <class T>
std::string toString(T t){
	std::stringstream ss;
	std::string s;
	ss << t;
	ss >> s;
	return s;
}
