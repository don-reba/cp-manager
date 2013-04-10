// Automatically generated file
#pragma once
#include "FastTrackIpc/Api/Api.h"
#include <map>
#include <set>
#include <stdexcept>
#include <stdint.h>
#include <vector>
#include <iostream>
class IProtocol;
class Tracker {
public:
  // interface
  Tracker(IProtocol & protocol);
public:
  // service function wrappers
  bool isPrime(int32_t n);
  std::vector<int32_t> factor(int32_t n, FactorizationMethod method);
  std::vector<char> searchByPair(std::vector<char> data);
private:
  // data
  IProtocol & protocol;
};
