#include "AdminServer.h"
#include "AdminID.h"
#include "GpuIpc/IProtocol.h"

#include <stdexcept>

// IProcess implementation
void AdminServer::process(IProtocol & protocol) {
  uint32_t id = protocol.readUInt32();
  switch (id) {
  case AdminID_Exit:
    process_Exit(protocol);
    break;
  default:
    throw std::runtime_error("AdminServer::process: unknown ID");
  }
}

// service function wrappers
void AdminServer::process_Exit(IProtocol &) {
  this->Exit();
}

