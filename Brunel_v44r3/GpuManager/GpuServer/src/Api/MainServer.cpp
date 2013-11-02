#include "MainServer.h"
#include "GpuIpc/IProtocol.h"

#include <stdexcept>
#include <iostream>

using namespace std;

//----------
// interface
//----------

MainServer::MainServer() {
  m_handlers["search by pair"] = &MainServer::process_searchByPair;
}

//------------------------
// IProcess implementation
//------------------------

void MainServer::process(IProtocol & protocol) {
  std::string handlerName = protocol.readString();
  size_t      size        = protocol.readUInt32();

  vector<uint8_t> data(size);
  protocol.readData(&data[0], size);

  HandlerMap::const_iterator i = m_handlers.find(handlerName);
  if (i != m_handlers.end()) {
    Handler handler = i->second;
    (this->*handler)(&data[0], size);
  } else {
    string msg = "invalid handler name: ";
    msg.append(handlerName);
    throw runtime_error(msg);
  }
}

//---------
// handlers
//---------

void MainServer::process_searchByPair(void *, size_t size) {
  cout << "searchByPair received " << size << " data" << endl;
}
