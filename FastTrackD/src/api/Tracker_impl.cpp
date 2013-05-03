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
}
void Tracker::factor(int32_t n, FactorizationMethod method, std::vector<int32_t> & result) const {
  // add function implementation here
}
void Tracker::searchByPair(const std::vector<int8_t> & data, std::vector<int8_t> & result) const {
}
