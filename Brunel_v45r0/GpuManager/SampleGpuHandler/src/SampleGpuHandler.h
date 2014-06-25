#include "GpuHandler/IGpuHandler.h"

class SampleGpuHandler : public IGpuHandler
{
  virtual void operator() (
      const Batch & batch,
      Alloc         allocResult,
      AllocParam    allocResultParam);

  static void printBatchInfo(const Batch & batch);
};
