// Automatically generated file
#include "AdminServer.h"
#include "AdminID.h"
#include "GpuIpc/IProtocol.h"
// IProcess implementation
bool AdminServer::process(IProtocol & protocol) {
  int id = protocol.readInt32();
  switch (id) {
  case AdminID_Exit:
    process_Exit(protocol);
    break;
  default:
    throw std::runtime_error("AdminServer::process: unknown ID");
  }
  return true;
}
// service function wrappers
void AdminServer::process_Exit(IProtocol & protocol) {
  this->Exit();
}
