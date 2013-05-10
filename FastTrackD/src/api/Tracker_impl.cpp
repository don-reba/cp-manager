// Automatically generated file
#include "Tracker.h"

#include <algorithm>
#include <iterator>
#include <numeric>

using namespace std;

GpuTrack CreateGpuTrack(int8_t seed) {
  GpuTrack track;
  track.X0  = 1.0;
  track.Tx  = 2.0;
  track.Y0  = 3.0;
  track.Ty  = 4.0;
  track.S0  = 5.0;
  track.Sx  = 6.0;
  track.Sz  = 7.0;
  track.Sxz = 8.0;
  track.Sz2 = 9.0;
  track.U0  = 10.0;
  track.Uy  = 11.0;
  track.Uz  = 12.0;
  track.Uyz = 13.0;
  track.Uz2 = 14.0;
  track.TrackHitsNum = seed;
  track.Hits.push_back(seed * 2);
  track.Hits.push_back(seed * 3);
  return track;
}
 
// service functions for the user to implement
void Tracker::searchByPair(const std::vector<int8_t> & data, std::vector<GpuTrack> & result) const {
  transform(data.begin(), data.end(), back_inserter(result), CreateGpuTrack);
}
