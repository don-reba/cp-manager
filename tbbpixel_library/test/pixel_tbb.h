
#ifndef TRACK
#define TRACK 1

#include <vector>
#include <iostream>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <stdlib.h>

template <class T>
static std::string toString(T t){
	std::stringstream ss;
	std::string s;
	ss << t;
	ss >> s;
	return s;
}

struct PixelEvent {
  int noSensors;
  int noHits;
  std::vector<int> sensorZs;
  std::vector<int> sensorHitStarts;
  std::vector<int> sensorHitsNums;
  std::vector<int> hitIDs;
  std::vector<float> hitXs;
  std::vector<float> hitYs;
  std::vector<int> hitZs;
};

struct GpuTrack {
  float x0;
  float tx;
  float y0;
  float ty;
  float s0;
  float sx;
  float sz;
  float sxz;
  float sz2;
  float u0;
  float uy;
  float uz;
  float uyz;
  float uz2;
  int trackHitsNum;
  std::vector<int> hits;
};

void pixel_tracker_implementation(const PixelEvent & data, std::vector<GpuTrack>& result);

#endif
