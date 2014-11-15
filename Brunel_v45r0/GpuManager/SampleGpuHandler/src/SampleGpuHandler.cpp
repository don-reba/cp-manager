#include "SampleGpuHandler.h"

#include <iostream>

using namespace std;

DECLARE_COMPONENT(SampleGpuHandler)

void SampleGpuHandler::operator() (
    const Batch & batch,
    Alloc         allocResult,
    AllocParam    allocResultParam) {
  // assign an index to each processed batch
  // and set it as the result
  static uint32_t callCount = 0;
  for (size_t i = 0, size = batch.size(); i != size; ++i) {
    *(uint32_t*)allocResult(i, 4, allocResultParam) = callCount;
    ++callCount;
  }
}
