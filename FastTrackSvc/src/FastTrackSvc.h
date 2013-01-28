#pragma once

#include "FastTrackSvc/IFastTrackSvc.h"

#include <GaudiKernel/Service.h>

class SocketClient;

class FastTrackSvc :
    public         Service,
    public virtual IFastTrackSvc {
  public:
    FastTrackSvc(const std::string & name, ISvcLocator * sl);
    virtual ~FastTrackSvc();

    // Service implementation
    virtual StatusCode queryInterface(const InterfaceID & riid, void ** ppvIF);
    virtual StatusCode initialize();
    virtual StatusCode finalize();

  private:
    void cleanup();
    void initIO();

  private:
    SocketClient * transport;
};
