#include "GpuHandler/IGpuHandler.h"

class PrPixelCudaHandler : public IGpuHandler
{
    virtual void operator() (
        const Batch & batch,
        Alloc         resultAlloc,
        AllocParam    resultAllocParam);
};
