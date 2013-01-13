#pragma once

#include "GaudiKernel/IInterface.h"

class IFastTrackSvc :
    public virtual IInterface {
  public:
    static const InterfaceID & interfaceID();

    virtual ~IFastTrackSvc() {}
};

inline const InterfaceID & IFastTrackSvc::interfaceID() {
  static InterfaceID id("IFastTrackSvc", 1, 0);
  return id;
}
