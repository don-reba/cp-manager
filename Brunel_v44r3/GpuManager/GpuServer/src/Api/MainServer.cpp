#include "TrackerServer.h"
#include "GpuIpc/IProtocol.h"

#include <stdexcept>
#include <iostream>

using namespace std;

//----------
// interface
//----------

MainServer::MainServer() {
  m_handlers["search by pair"] = process_searchByPair;
}

//------------------------
// IProcess implementation
//------------------------

void TrackerServer::process(IProtocol & protocol) {
  std::string handlerName = protocol.readString();
  size_t      size        = protocol.readUInt32();

  vector<uint8_t> data(size);
  protocol.readData(&data[0], size);

  HandlerMap::const_iter i = m_handlers.find(handlerName);
  if (i != m_handlers.end()) {
    (*i->second)(&data[0], size);
  } else {
    string msg = "invalid handler name: ";
    msg.append(handlerName);
    throw exception(msg);
  }
}

//---------
// handlers
//---------

void TrackerServer::process_searchByPair(void * data, size_t size) {
  cout << "searchByPair received " << size << " data" << endl;
}
