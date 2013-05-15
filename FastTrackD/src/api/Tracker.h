// Automatically generated file
#pragma once
#include <map>
#include <set>
#include <stdexcept>
#include <stdint.h>
#include <vector>
#include "FastTrackIpc/api/api.h"
#include "FastTrackIpc/IProcessor.h"
class IProtocol;
class Tracker : public IProcessor {
public:
  // IProcess implementation
  virtual bool process(IProtocol & protocol) const;
private:
  // service functions for the user to implement
  void searchByPair(const std::vector<int8_t> & data, std::vector<GpuTrack> & result) const;
private:
  // generated wrappers
  void process_searchByPair(IProtocol & protocol) const;
};
