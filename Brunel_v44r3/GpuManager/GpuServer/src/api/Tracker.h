// Automatically generated file
#pragma once
#include <map>
#include <set>
#include <stdexcept>
#include <stdint.h>
#include <vector>
#include "GpuIpc/api/api.h"
#include "GpuIpc/IProcessor.h"
class IProtocol;
class Tracker : public IProcessor {
public:
  // IProcess implementation
  virtual bool process(IProtocol & protocol) const;
private:
  // service functions for the user to implement
  void searchByPair(const PixelEvent & event, std::vector<GpuTrack> & tracks) const;
private:
  // generated wrappers
  void process_searchByPair(IProtocol & protocol) const;
};
