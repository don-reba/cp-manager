#include "GpuService.h"

#include "api/Tracker.h"

#include "GpuIpc/SocketClient.h"
#include "GpuIpc/Protocol.h"

#include <GaudiKernel/SvcFactory.h>

#include <iostream>

DECLARE_SERVICE_FACTORY(GpuService)

//-------------
// construction
//-------------

GpuService::GpuService(const std::string & name, ISvcLocator * sl) :
    Service     (name, sl),
    m_transport (NULL),
    m_protocol  (NULL),
    m_tracker   (NULL) {
}

GpuService::~GpuService() {
}

//-----------------------------
// IGpuService implementation
//-----------------------------

std::vector<GpuTrack> GpuService::searchByPair(const std::vector<char> & trackerInputData) {
  // TODO: Efficient?
  const std::vector<int8_t> convertedDataPointer(trackerInputData.begin(), trackerInputData.end());
  
  std::vector<GpuTrack> result;
  m_tracker->searchByPair(convertedDataPointer, result);

  return result;
}

//-----------------------
// Service implementation
//-----------------------

StatusCode GpuService::queryInterface(const InterfaceID & riid, void ** ppvIF) {
  if (riid == IGpuService::interfaceID()) {
    *ppvIF = dynamic_cast<IGpuService*>(this);
    return StatusCode::SUCCESS;
  }
  return Service::queryInterface(riid, ppvIF);
}

StatusCode GpuService::initialize() {
  StatusCode sc(Service::initialize());
  if (StatusCode::SUCCESS != sc)
    return sc;

  MsgStream msg(msgSvc(), name());
  msg << MSG::INFO << "GpuService::initialize()" << endmsg;

  initIO();

  return StatusCode::SUCCESS;
}

StatusCode GpuService::finalize() {
  MsgStream msg(msgSvc(), name());
  msg << MSG::INFO << "GpuService::finalize()" << endmsg;

  cleanup();

  return StatusCode::SUCCESS;
}

void GpuService::cleanup() {
  if (m_tracker != NULL)
    delete m_tracker;
  if (m_protocol != NULL)
    delete m_protocol;
  if (m_transport != NULL)
    delete m_transport;
}

void GpuService::initIO() {
  m_transport = new SocketClient("/tmp/GpuManager");
  m_protocol  = new Protocol(*m_transport);
  m_tracker   = new Tracker(*m_protocol);
}
