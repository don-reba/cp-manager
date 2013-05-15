// Automatically generated file
#include "Tracker.h"

#include <algorithm>
#include <iterator>
#include <numeric>

using namespace std;

GpuTrack CreateGpuTrack(int8_t seed) {
  GpuTrack track;
  track.x0  = 1.0;
  track.tx  = 2.0;
  track.y0  = 3.0;
  track.ty  = 4.0;
  track.s0  = 5.0;
  track.sx  = 6.0;
  track.sz  = 7.0;
  track.sxz = 8.0;
  track.sz2 = 9.0;
  track.u0  = 10.0;
  track.uy  = 11.0;
  track.uz  = 12.0;
  track.uyz = 13.0;
  track.uz2 = 14.0;
  track.trackHitsNum = seed;
  track.hits.push_back(seed * 2);
  track.hits.push_back(seed * 3);
  return track;
}
 
// service functions for the user to implement
void Tracker::searchByPair(const std::vector<int8_t> & data, std::vector<GpuTrack> & result) const {
  transform(data.begin(), data.end(), back_inserter(result), CreateGpuTrack);
}
