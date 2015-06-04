#pragma once

#include <cstdint>
#include <vector>

#define MAX_TRACK_SIZE 24
struct GpuTrack {
  float x0;
  float tx;
  float y0;
  float ty;
  uint32_t hitsNum;
  uint32_t hits[MAX_TRACK_SIZE];
};

struct PixelEvent {
  int32_t noSensors;
  int32_t noHits;
  std::vector<int32_t> sensorZs;
  std::vector<int32_t> sensorHitStarts;
  std::vector<int32_t> sensorHitsNums;
  std::vector<int32_t> hitIDs;
  std::vector<float>   hitXs;
  std::vector<float>   hitYs;
  std::vector<float>   hitZs;
};
