#pragma once

#include <GaudiKernel/IInterface.h>

#include "FastTrackIpc/api/api.h"

/** @class IFastTrackSvc IFastTrackSvc.h FastTrackSvc/IFastTrackSvc.h
 * Base interface providing accelerated tracking algorithms.
 */
class IFastTrackSvc :
    public virtual IInterface {
  public:
    virtual ~IFastTrackSvc() {}

    virtual void searchByPair(const std::vector<GpuTrack> & data, std::vector<int8_t> & result) = 0;

    DeclareInterfaceID(IFastTrackSvc, 1, 0);
};
