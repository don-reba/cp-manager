#pragma once

#include "GaudiKernel/IInterface.h"
#include "GpuIpc/Api/Api.h"

/** @class IGpuService IGpuService.h GpuService/IGpuService.h
 * Base interface providing GPU-accelerated algorithms.
 */
class IGpuService :
    public virtual IInterface {
  public:
    virtual ~IGpuService() {}

    virtual std::vector<GpuTrack> searchByPair(const std::vector<char> & trackerInputData) = 0;

    DeclareInterfaceID(IGpuService, 1, 0);
};
