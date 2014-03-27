#pragma once

#include <vector>

#include <stdint.h>

struct GpuTrack {
  float   x0;
  float   tx;
  float   y0;
  float   ty;

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
};
