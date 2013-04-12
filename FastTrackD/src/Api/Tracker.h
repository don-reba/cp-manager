// Automatically generated file
#pragma once
#include <map>
#include <set>
#include <stdexcept>
#include <stdint.h>
#include <vector>
#include "FastTrackIpc/Api/Api.h"
#include "FastTrackIpc/IProcessor.h"
class IProtocol;
class Tracker : public IProcessor {
public:
  // IProcess implementation
  virtual bool process(IProtocol & protocol) const;
private:
  // service functions for the user to implement
  bool isPrime(int32_t n) const;
  std::vector<int32_t> factor(int32_t n, FactorizationMethod method) const;
  std::vector<uint8_t> searchByPair(std::vector<uint8_t> data) const;
private:
  // generated wrappers
  void process_isPrime(IProtocol & protocol) const;
  void process_factor(IProtocol & protocol) const;
  void process_searchByPair(IProtocol & protocol) const;
};
