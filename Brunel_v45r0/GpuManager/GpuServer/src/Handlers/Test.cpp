#include "Test.h"

#include <iostream>

using namespace Handlers;
using namespace std;

namespace {
  void printBatchInfo(const Batch & batch) {
    cout << "'test' received";
    for (size_t i = 0, size = batch.size(); i != size; ++i)
      cout << ' ' << batch[i]->size();
    cout << " bytes" << endl;
  }
}

void Handlers::test(
    const Batch & batch,
    Alloc         allocResult,
    AllocParam    allocResultParam) {
  printBatchInfo(batch);

  static uint32_t callCount = 0;
  for (size_t i = 0, size = batch.size(); i != size; ++i) {
    *(uint32_t*)allocResult(i, 4, allocResultParam) = callCount;
    ++callCount;
  }
}
