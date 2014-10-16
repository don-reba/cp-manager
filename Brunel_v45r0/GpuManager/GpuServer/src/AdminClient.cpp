#include "AdminClient.h"
#include "AdminID.h"
#include "GpuIpc/IProtocol.h"

using namespace std;

AdminClient::AdminClient(IProtocol & protocol) : protocol(protocol) {
}

void AdminClient::exit() {
  protocol.writeUInt32(AdminID_Exit);
}

void AdminClient::loadHandler(const string & handlerName) {
  protocol.writeUInt32(AdminID_LoadHandler);
  protocol.writeString(handlerName);
}
