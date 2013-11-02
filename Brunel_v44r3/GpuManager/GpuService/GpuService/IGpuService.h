#pragma once

#include "GaudiKernel/IInterface.h"

/** @class IGpuService IGpuService.h GpuService/IGpuService.h
 * Base interface providing GPU-accelerated algorithms.
 */
class IGpuService :
    public virtual IInterface {
  public:
    virtual ~IGpuService() {}

    virtual void submitData(
        std::string  handlerName,
        const void * data,
        const size_t size) = 0;

    DeclareInterfaceID(IGpuService, 1, 0);
};
