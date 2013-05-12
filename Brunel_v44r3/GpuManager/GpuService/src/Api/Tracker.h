// Automatically generated file
#pragma once
#include "GpuIpc/Api/Api.h"
#include <map>
#include <set>
#include <stdexcept>
#include <stdint.h>
#include <vector>
class IProtocol;
class Tracker {
public:
  // interface
  Tracker(IProtocol & protocol);
public:
  // service function wrappers
  void searchByPair(std::vector<int8_t> data, std::vector<GpuTrack> & result);
private:
  // data
  IProtocol & protocol;
};
