#pragma once

#include "GaudiKernel/IInterface.h"

/** @class IGpuService IGpuService.h GpuService/IGpuService.h
 * Base interface providing GPU-accelerated algorithms.
 */
class IGpuService :
    public virtual IInterface {
  public:
    virtual ~IGpuService() {}

    virtual bool isPrime(int n) = 0;

    virtual void searchByPair(const std::vector<char> & tracks) = 0;

    DeclareInterfaceID(IGpuService, 1, 0);
};
