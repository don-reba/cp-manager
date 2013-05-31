// Automatically generated file
#include "Admin.h"
#include "AdminID.h"
#include "GpuIpc/IProtocol.h"
Admin::Admin(IProtocol & protocol) : protocol(protocol) {
}
// service function wrappers
void Admin::Exit() {
  protocol.writeInt32(AdminID_Exit);
}
