#include "FastTrackSvc.h"

#include "Api/Tracker.h"

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

bool FastTrackSvc::isPrime(int n) {
  return m_tracker->isPrime(n);
}

void FastTrackSvc::searchByPair(const std::vector<char> & tracks) {
  const std::vector<int8_t> converted(tracks.begin(), tracks.end());
  m_tracker->searchByPair(converted);
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
  m_transport = new SocketClient("/tmp/FastTrackDR2");
  m_protocol  = new Protocol(*m_transport);
  m_tracker   = new Tracker(*m_protocol);
}
