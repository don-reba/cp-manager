// Automatically generated file
#include "Tracker.h"

#include <algorithm>
#include <iterator>
#include <numeric>

using namespace std;

int Multiply(int a, int b) {
  return a * b;
}

int MultiplyHits(const GpuTrack & track) {
  return accumulate(track.Hits.begin(), track.Hits.end(), 1, Multiply);
}
 
// service functions for the user to implement
void Tracker::searchByPair(const std::vector<GpuTrack> & data, std::vector<int8_t> & result) const {
  transform(data.begin(), data.end(), back_inserter(result), MultiplyHits);
}
