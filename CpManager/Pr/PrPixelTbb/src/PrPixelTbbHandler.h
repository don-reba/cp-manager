#pragma once

#include "CpHandler/ICpHandler.h"

class PrPixelTbbHandler : public ICpHandler
{
    virtual void operator() (
        const Batch & batch,
        Alloc         allocResult,
        AllocParam    allocResultParam);
};
