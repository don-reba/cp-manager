#include "FastTrackSvc.h"

#include "api/Tracker.h"

#include "FastTrackIpc/SocketClient.h"
#include "FastTrackIpc/Protocol.h"

#include <GaudiKernel/SvcFactory.h>

#include <iostream>

DECLARE_SERVICE_FACTORY(FastTrackSvc)

//-------------
// construction
//-------------

FastTrackSvc::FastTrackSvc(const std::string & name, ISvcLocator * sl) :
    Service     (name, sl),
    m_transport (NULL),
    m_protocol  (NULL),
    m_tracker   (NULL) {
}

FastTrackSvc::~FastTrackSvc() {
}

//-----------------------------
// IFastTrackSvc implementation
//-----------------------------

void FastTrackSvc::searchByPair(const std::vector<int8_t> & data, std::vector<GpuTrack> & result) {
  m_tracker->searchByPair(data, result);
}

//-----------------------
// Service implementation
//-----------------------

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

  initIO();

  return StatusCode::SUCCESS;
}

StatusCode FastTrackSvc::finalize() {
  MsgStream msg(msgSvc(), name());
  msg << MSG::INFO << "FastTrackSvc::finalize()" << endmsg;

  cleanup();

  return StatusCode::SUCCESS;
}

void FastTrackSvc::cleanup() {
  if (m_tracker != NULL)
    delete m_tracker;
  if (m_protocol != NULL)
    delete m_protocol;
  if (m_transport != NULL)
    delete m_transport;
}

void FastTrackSvc::initIO() {
  m_transport = new SocketClient("/tmp/FastTrack");
  m_protocol  = new Protocol(*m_transport);
  m_tracker   = new Tracker(*m_protocol);
}
