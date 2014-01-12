#include "DataLog.h"
#include "GpuIpc/IProtocol.h"
#include "MainServer.h"
#include "PerfLog.h"
#include "Timer.h"

#include "Handlers/PatPixel.h"
#include "Handlers/Test.h"

#include <algorithm>
#include <cstring>
#include <ctime>
#include <iostream>
#include <sstream>
#include <stdexcept>

using namespace boost;
using namespace std;

//----------
// interface
//----------

MainServer::MainServer(PerfLog & perfLog, DataLog & dataLog) :
    m_perfLog (perfLog),
    m_dataLog (dataLog) {
  m_handlers["searchByPair"] = &Handlers::searchByPair;
  m_handlers["test"]         = &Handlers::test;
}

//------------------------
// IProcess implementation
//------------------------

void MainServer::process(IProtocol & protocol) {
  const size_t FAIL_FLAG = 0xFFFFFFFF;

  std::string handlerName = protocol.readString();

  bool isProfiling = protocol.readBool();

  size_t size = protocol.readUInt32();

  Data data(size);
  protocol.readData(&data[0], size);

  m_dataLog.addRecord(handlerName, data);

  HandlerMap::const_iterator i = m_handlers.find(handlerName);
  if (i == m_handlers.end()) {
    // when a handler is not found, inform the client
    protocol.writeUInt32(FAIL_FLAG);
    protocol.writeString(createInvalidHandlerMsg(handlerName));
    return;
  }

  // call the handler
  Data result;
  DataPacket packet(handlerName, &data, &result);
  m_dataQueue.push(&packet);

  Timer timer;
  timer.start();
  packet.Wait();
  timer.stop();

  if (packet.ExceptionThrown()) {
    protocol.writeUInt32(FAIL_FLAG);
    protocol.writeString(packet.ExceptionMessage());
    return;
  }

  // return the result to client
  protocol.writeUInt32(result.size());
  protocol.writeData(&result[0], result.size());

  if (isProfiling) {
    double kernelSeconds = packet.Seconds();
    double idleSeconds   = timer.secondsElapsed() - kernelSeconds;
    double totalSeconds  = protocol.readDouble() - idleSeconds;
    m_perfLog.addRecord(time(0), handlerName.c_str(), totalSeconds, kernelSeconds, size, result.size());
  }
}

void MainServer::start() {
    m_processingThread = thread(&MainServer::processQueue, this);
}

void MainServer::stop() {
  m_dataQueue.interrupt();
}

//------------------
// Private functions
//------------------

void * MainServer::allocVector(size_t size, AllocParam param) {
  Data * data = reinterpret_cast<Data*>(param);
  data->resize(size);
  return &data->at(0);
}

string MainServer::createInvalidHandlerMsg(const string & handler) const {
    ostringstream msg;
    msg << "invalid handler name: " << handler << "; ";
    msg << "valid handlers: ";
    for (HandlerMap::const_iterator i = m_handlers.begin(), end = m_handlers.end(); i != end; ++i) {
      if (i != m_handlers.begin())
        msg << ", ";
      msg << i->first;
    }
    msg << ".";
    return msg.str();
}

void MainServer::processQueue()
try {
  // the data queue throws an exception when interrupted
  while (true) {
    DataPacket * packet = m_dataQueue.pop();

    Timer timer;

    // get handler by name
    HandlerMap::const_iterator i = m_handlers.find(packet->Name());
    if (i == m_handlers.end())
      throw runtime_error(createInvalidHandlerMsg(packet->Name()));
    Handler handler = i->second;

    // execute handler
    try {
      timer.start();
      (*handler)(*packet->Data(), allocVector, packet->Result());
      timer.stop();
    } catch (const std::exception & e) {
      packet->SetExceptionMessage(e.what());
    }

    packet->SetSeconds(timer.secondsElapsed());

    packet->Signal();
  }
} catch (const Queue::interrupted_error &) {
  // it's ok
}
