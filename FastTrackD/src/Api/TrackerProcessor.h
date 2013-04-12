// Automatically generated file
#pragma once
#include <map>
#include <set>
#include <stdexcept>
#include <stdint.h>
#include <vector>
#include <iostream>
#include "FastTrackIpc/Api/Api.h"
#include "FastTrackIpc/IProcessor.h"

#include "Tracker.h"

class IProtocol;
class TrackerProcessor : public IProcessor {
public:
  
  TrackerProcessor();

  // IProcess implementation
  virtual bool process(IProtocol & protocol) const;

private:

  mutable Tracker _t;

  bool isPrime(int32_t n) const;
  std::vector<int32_t> factor(int32_t n, FactorizationMethod method) const;

  void process_isPrime(IProtocol & protocol) const;
  void process_factor(IProtocol & protocol) const;
  void process_searchByPair(IProtocol & protocol) const;
};
