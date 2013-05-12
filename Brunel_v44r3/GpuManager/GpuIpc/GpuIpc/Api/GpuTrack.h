// Automatically generated file
#pragma once
#include <map>
#include <set>
#include <stdexcept>
#include <stdint.h>
#include <vector>
class IProtocol;
struct GpuTrack {
  double x0;
  double tx;
  double y0;
  double ty;
  double s0;
  double sx;
  double sz;
  double sxz;
  double sz2;
  double u0;
  double uy;
  double uz;
  double uyz;
  double uz2;
  int32_t trackHitsNum;
  std::vector<int32_t> hits;
  void read(IProtocol & protocol);
  void write(IProtocol & protocol) const;
};
