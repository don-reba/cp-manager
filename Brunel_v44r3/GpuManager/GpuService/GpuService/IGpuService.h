#pragma once

#include "GaudiKernel/IInterface.h"

/** @class IGpuService IGpuService.h GpuService/IGpuService.h
 * Base interface providing GPU-accelerated algorithms.
 */
class IGpuService :
    public virtual IInterface {
  public:
    typedef void * AllocParam;
    typedef void * (*Alloc)(size_t size, AllocParam param);

  public:
    virtual ~IGpuService() {}

    virtual void submitData(
        std::string  handlerName,
        const void * data,
        const size_t size,
        Alloc        allocResults,
        void *       allocResultsParam) = 0;

    DeclareInterfaceID(IGpuService, 1, 0);
};
