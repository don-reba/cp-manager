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
  bool isPrime(int32_t n) const;
  void factor(int32_t n, FactorizationMethod method, std::vector<int32_t> & result) const;
  void searchByPair(std::vector<int8_t> data, std::vector<int8_t> & result) const;
private:
  // generated wrappers
  void process_isPrime(IProtocol & protocol) const;
  void process_factor(IProtocol & protocol) const;
  void process_searchByPair(IProtocol & protocol) const;
};
