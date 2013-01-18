#pragma once

#include "GaudiKernel/IInterface.h"

/** @class IFastTrackSvc IFastTrackSvc.h FastTrackSvc/IFastTrackSvc.h
 * Base interface providing accelerated tracking algorithms.
 */
class IFastTrackSvc :
    public virtual IInterface {
  public:
    virtual ~IFastTrackSvc() {}

    DeclareInterfaceID(IFastTrackSvc, 1, 0);
};
