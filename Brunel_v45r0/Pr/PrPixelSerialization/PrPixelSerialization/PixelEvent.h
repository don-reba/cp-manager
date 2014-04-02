#pragma once

#include <vector>

#include <stdint.h>

struct PixelEvent {
  int32_t              noSensors;
  int32_t              noHits;
  std::vector<int32_t> sensorZs;
  std::vector<int32_t> sensorHitStarts;
  std::vector<int32_t> sensorHitsNums;
  std::vector<int32_t> hitIDs;
  std::vector<float>   hitXs;
  std::vector<float>   hitYs;
  std::vector<int32_t> hitZs;
};
