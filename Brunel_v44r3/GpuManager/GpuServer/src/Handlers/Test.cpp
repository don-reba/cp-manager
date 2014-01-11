#include "Test.h"

#include <iostream>

using namespace std;

void Handlers::test(
    const Data & data,
    Alloc        allocResult,
    AllocParam   allocResultParam) {
  cout << "'test' received " << data.size() << " bytes" << endl;

  static uint32_t callCount = 0;
  *(uint32_t*)allocResult(4, allocResultParam) = callCount;
  ++callCount;
}
