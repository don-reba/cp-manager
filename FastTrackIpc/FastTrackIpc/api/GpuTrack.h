// Automatically generated file
#pragma once
#include <map>
#include <set>
#include <stdexcept>
#include <stdint.h>
#include <vector>
class IProtocol;
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
  int32_t trackHitsNum;
  std::vector<int32_t> hits;
  void read(IProtocol & protocol);
  void write(IProtocol & protocol) const;
};
