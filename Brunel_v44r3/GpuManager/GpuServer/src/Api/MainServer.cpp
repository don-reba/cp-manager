#include "MainServer.h"
#include "GpuIpc/IProtocol.h"

#include <cstring>
#include <stdexcept>
#include <iostream>
#include <vector>

using namespace std;

//----------
// interface
//----------

MainServer::MainServer() {
  m_handlers["searchByPair"] = &MainServer::process_SearchByPair;
  m_handlers["test"]         = &MainServer::process_Test;
}

//------------------------
// IProcess implementation
//------------------------

void * MainServer::vectorAlloc(size_t size, AllocParam param) {
  typedef vector<uint8_t> Data;
  Data * data = reinterpret_cast<Data*>(param);
  data->resize(size);
  return &data->at(0);
}

void MainServer::process(IProtocol & protocol) {
  std::string handlerName = protocol.readString();

  size_t size = protocol.readUInt32();

  vector<uint8_t> data(size);
  protocol.readData(&data[0], size);

  HandlerMap::const_iterator i = m_handlers.find(handlerName);
  if (i != m_handlers.end()) {
    // call the handler and return the result to client
    vector<uint8_t> result;

    Handler handler = i->second;
    (this->*handler)(&data[0], size, vectorAlloc, &result);

    protocol.writeUInt32(result.size());
    protocol.writeData(&result[0], result.size());
  } else {
    // when a handler is not found, return an error flag instead of the size
    // and an error message to client
    const size_t FAIL_FLAG = 0xFFFFFFFF;
    protocol.writeUInt32(FAIL_FLAG);

    string msg = "invalid handler name: ";
    msg.append(handlerName);
    protocol.writeString(msg);
  }
}

//---------
// handlers
//---------

void MainServer::process_SearchByPair(
    void *     ,
    size_t     size,
    Alloc      allocResult,
    AllocParam allocResultParam) {
  cout << "'searchByPair' received " << size << " data" << endl;
  *(uint32_t*)allocResult(4, allocResultParam) = 0;
}

void MainServer::process_Test(
    void *     ,
    size_t     size,
    Alloc      allocResult,
    AllocParam allocResultParam) {
  cout << "'test' received " << size << " data" << endl;

  static uint32_t callCount = 0;
  *(uint32_t*)allocResult(4, allocResultParam) = callCount;
  ++callCount;
}
