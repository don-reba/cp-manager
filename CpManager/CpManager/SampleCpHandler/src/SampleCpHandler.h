#include "CpHandler/ICpHandler.h"

class SampleCpHandler : public ICpHandler
{
  virtual void operator() (
      const Batch & batch,
      Alloc         allocResult,
      AllocParam    allocResultParam);

  static void printBatchInfo(const Batch & batch);
};
