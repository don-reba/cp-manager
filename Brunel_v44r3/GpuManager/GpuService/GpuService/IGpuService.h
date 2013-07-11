#pragma once

#include "GaudiKernel/IInterface.h"
#include "GpuIpc/api/api.h"

/** @class IGpuService IGpuService.h GpuService/IGpuService.h
 * Base interface providing GPU-accelerated algorithms.
 */
class IGpuService :
    public virtual IInterface {
  public:
    virtual ~IGpuService() {}

    virtual void searchByPair(
      const PixelEvent      & event,
      std::vector<GpuTrack> & tracks) = 0;

    DeclareInterfaceID(IGpuService, 1, 0);
};
