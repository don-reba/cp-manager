// Automatically generated file
#pragma once
#include <map>
#include <set>
#include <stdexcept>
#include <stdint.h>
#include <vector>
class IProtocol;
struct GpuTrack {
  double X0;
  double Tx;
  double Y0;
  double Ty;
  double S0;
  double Sx;
  double Sz;
  double Sxz;
  double Sz2;
  double U0;
  double Uy;
  double Uz;
  double Uyz;
  double Uz2;
  int32_t TrackHitsNum;
  std::vector<int32_t> Hits;
  void read(IProtocol & protocol);
  void write(IProtocol & protocol) const;
};
