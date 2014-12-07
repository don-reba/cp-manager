#pragma once

#include "GpuHandler/IGpuHandler.h"

class PrPixelTbbHandler : public IGpuHandler
{
    virtual void operator() (
        const Batch & batch,
        Alloc         allocResult,
        AllocParam    allocResultParam);
};
