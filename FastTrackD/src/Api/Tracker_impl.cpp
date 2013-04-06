// Automatically generated file
#include "Tracker.h"
// service functions for the user to implement
bool Tracker::isPrime(int32_t n) const {
  if (n < 2)
    return false;
  for (int i = 2; i != n; ++i) {
    if (n % i == 0)
      return false;
  }
  return true;
};
std::vector<int32_t> Tracker::factor(int32_t, FactorizationMethod) const {
  return std::vector<int32_t>();
};
