#include "Tracker.h"

#include "TrackerHelper.h"

//--------------------------------------------
// service functions for the user to implement
//--------------------------------------------

TrackerHelper helper;

bool Tracker::isPrime(int32_t n) const {
  if (n < 2)
    return false;
  for (int i = 2; i != n; ++i) {
    if (n % i == 0)
      return false;
  }
  return true;
};

std::vector<int32_t> Tracker::factor(int32_t n, FactorizationMethod method) const {
   return std::vector<int32_t>();
};

std::vector<uint8_t> Tracker::searchByPair(std::vector<uint8_t> data) const {
  helper.searchByPair(data);
};
