#include "GpuService.h"

#include "GpuIpc/SocketClient.h"
#include "GpuIpc/Protocol.h"

#include <GaudiKernel/SvcFactory.h>

#include <iostream>
#include <stdexcept>
#include <string>

using namespace std;

DECLARE_SERVICE_FACTORY(GpuService)

//-------------
// construction
//-------------

GpuService::GpuService(const std::string & name, ISvcLocator * sl) :
    Service      (name, sl),
    m_transport  (NULL),
    m_protocol   (NULL),
    m_socketPath ("/tmp/GpuManager") {
  declareProperty("SocketPath", m_socketPath);
}

GpuService::~GpuService() {
}

//-----------------------------
// IGpuService implementation
//-----------------------------

void GpuService::submitData(
    std::string  handlerName,
    const void * data,
    const size_t size,
    Alloc        allocResults,
    AllocParam   allocResultsParam) {
  // send the name of the addressee, followed by the data package
  m_protocol->writeString(handlerName);
  m_protocol->writeUInt32(size);
  m_protocol->writeData(data, size);

  size_t resultSize = m_protocol->readUInt32();

  // handle errors
  const size_t FAIL_FLAG = 0xFFFFFFFF;
  if (resultSize == FAIL_FLAG) {
    string message = m_protocol->readString();
    throw runtime_error(message);
  }

  // zero data size is normal
  if (resultSize > 0) {
    void * resultData = allocResults(resultSize, allocResultsParam);
    if (resultData == NULL) {
      // allocate a temporary buffer, if the client won't cooperate
      vector<uint8_t> temp(resultSize);
      m_protocol->readData(&temp[0], resultSize);
    } else {
      m_protocol->readData(resultData, resultSize);
    }
  }
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
  if (m_protocol != NULL)
    delete m_protocol;
  if (m_transport != NULL)
    delete m_transport;
}

void GpuService::initIO() {
  string socketPath = m_socketPath.value() + "-tracker";
  m_transport = new SocketClient(socketPath.c_str());
  m_protocol  = new Protocol(*m_transport);
}
