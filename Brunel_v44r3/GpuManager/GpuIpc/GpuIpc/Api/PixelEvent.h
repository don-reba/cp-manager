// Automatically generated file
#pragma once
#include <map>
#include <set>
#include <stdexcept>
#include <stdint.h>
#include <vector>
class IProtocol;
struct PixelEvent {
  int32_t noSensors;
  int32_t noHits;
  std::vector<int32_t> sensorZs;
  std::vector<int32_t> sensorHitStarts;
  std::vector<int32_t> sensorHitsNums;
  std::vector<int32_t> hitIDs;
  std::vector<float> hitXs;
  std::vector<float> hitYs;
  std::vector<int32_t> hitZs;
  void read(IProtocol & protocol);
  void write(IProtocol & protocol) const;
};
