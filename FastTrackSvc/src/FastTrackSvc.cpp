#include "FastTrackSvc.h"
#include "GaudiKernel/SvcFactory.h"

#include <iostream>

DECLARE_SERVICE_FACTORY(FastTrackSvc)

FastTrackSvc::FastTrackSvc(const std::string & name, ISvcLocator * sl) :
    Service(name, sl) {
}

FastTrackSvc::~FastTrackSvc() {
}

StatusCode FastTrackSvc::queryInterface(const InterfaceID & riid, void ** ppvIF) {
  if (riid == IFastTrackSvc::interfaceID()) {
    *ppvIF = dynamic_cast<IFastTrackSvc*>(this);
    return StatusCode::SUCCESS;
  }
  return Service::queryInterface(riid, ppvIF);
}

StatusCode FastTrackSvc::initialize() {
  StatusCode sc(Service::initialize());
  if (StatusCode::SUCCESS != sc)
    return sc;

  MsgStream msg(msgSvc(), name());
 
  msg << MSG::INFO << "FastTrackSvc::initialize()" << endmsg;

  return StatusCode::SUCCESS;
}
