// Automatically generated file
#pragma once
#include "FastTrackIpc/api/api.h"
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
  bool isPrime(int32_t n);
  void factor(int32_t n, FactorizationMethod method, std::vector<int32_t> & result);
  void searchByPair(const std::vector<int8_t> & data, std::vector<int8_t> & result);
private:
  // data
  IProtocol & protocol;
};
