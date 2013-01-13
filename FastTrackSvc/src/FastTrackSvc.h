#pragma once

#include "FastTrackSvc/IFastTrackSvc.h"
#include "GaudiKernel/Service.h"

class FastTrackSvc :
    public         Service,
    public virtual IFastTrackSvc {
  public:
    FastTrackSvc(const std::string & name, ISvcLocator * sl);
    virtual ~FastTrackSvc();

    // Service implementation
    StatusCode queryInterface(const InterfaceID & riid, void ** ppvIF);
    StatusCode initialize();
};
