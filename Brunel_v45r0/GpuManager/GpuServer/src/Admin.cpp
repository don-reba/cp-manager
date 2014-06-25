#include "Admin.h"
#include "AdminID.h"
#include "GpuIpc/IProtocol.h"

using namespace std;

Admin::Admin(IProtocol & protocol) : protocol(protocol) {
}

void Admin::exit() {
  protocol.writeUInt32(AdminID_Exit);
}

void Admin::loadHandler(const string & handlerName) {
	protocol.writeUInt32(AdminID_LoadHandler);
	protocol.writeString(handlerName);
}
