#include "CpHandler/ICpHandler.h"

class PrPixelCudaHandler : public ICpHandler
{
    virtual void operator() (
        const Batch & batch,
        Alloc         allocResult,
        AllocParam    allocResultParam);
};
