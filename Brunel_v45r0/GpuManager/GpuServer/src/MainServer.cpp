#include "DataLog.h"
#include "GpuIpc/IProtocol.h"
#include "MainServer.h"
#include "PerfLog.h"
#include "Timer.h"

#include "Handlers/PrPixel.h"
#include "Handlers/Test.h"

#include <algorithm>
#include <cstring>
#include <ctime>
#include <iostream>
#include <numeric>
#include <sstream>
#include <stdexcept>

using namespace boost;
using namespace Handlers;
using namespace std;

//----------
// interface
//----------

MainServer::MainServer(PerfLog & perfLog, DataLog & dataLog) :
    m_perfLog (perfLog),
    m_dataLog (dataLog) {
  m_handlers["tripletSearchGPU"] = &Handlers::tripletSearchGPU;
  m_handlers["test"]          = &Handlers::test;
}

//------------------------
// IProcess implementation
//------------------------

void MainServer::process(IProtocol & protocol) {
  const size_t FAIL_FLAG = 0xFFFFFFFF;

  std::string handlerName = protocol.readString();

  size_t size = protocol.readUInt32();

  Data input(size);
  protocol.readData(&input[0], size);

  HandlerMap::const_iterator i = m_handlers.find(handlerName);
  if (i == m_handlers.end()) {
    // when a handler is not found, inform the client
    protocol.writeUInt32(FAIL_FLAG);
    protocol.writeString(createInvalidHandlerMsg(handlerName));
    return;
  }

  // call the handler
  Data output;
  DataPacket packet(handlerName, &input, &output);
  m_dataQueue.push(&packet);

  Timer timer;
  timer.start();
  packet.Wait();
  timer.stop();

  // forward exceptions to client
  if (packet.ExceptionThrown()) {
    protocol.writeUInt32(FAIL_FLAG);
    protocol.writeString(packet.ExceptionMessage());
    return;
  }

  m_dataLog.addRecord(handlerName, input, output);

  // return the output to client
  protocol.writeUInt32(output.size());
  protocol.writeData(&output[0], output.size());
  protocol.writeDouble(timer.secondsElapsed());
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

size_t MainServer::addSize(size_t total, const Data * data) {
  return total + data->size();
}

void * MainServer::allocVector(size_t index, size_t size, AllocParam param) {
  typedef vector<Data*> Batch;
  Batch * batch = reinterpret_cast<Batch*>(param);
  Data * data = batch->at(index);
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
    string              name;
    vector<DataPacket*> batch;
    m_dataQueue.pop(name, batch);

    // get handler by name
    HandlerMap::const_iterator i = m_handlers.find(name);
    if (i == m_handlers.end())
      throw runtime_error(createInvalidHandlerMsg(name));
    Handler handler = i->second;

    // prepare data
    vector<const Data*> input  (batch.size());
    vector<Data*>       output (batch.size());
    for (size_t i = 0, size = batch.size(); i != size; ++i) {
      input[i]  = batch[i]->Input();
      output[i] = batch[i]->Output();
    }

    Timer timer;

    try {
      // execute handler
      timer.start();
      (*handler)(input, allocVector, &output);
      timer.stop();
    } catch (const std::exception & e) {
      // propagate the exception to all client in the batch
      for (size_t i = 0, size = batch.size(); i != size; ++i) {
        batch[i]->SetExceptionMessage(e.what());
      }
    }

    // gather statistics
    double secondsElapsed  = timer.secondsElapsed();
    size_t totalInputSize  = accumulate(input.begin(),  input.end(),  0u, addSize);
    size_t totalOutputSize = accumulate(output.begin(), output.end(), 0u, addSize);
    m_perfLog.addRecord(
        time(0), name.c_str(), secondsElapsed, totalInputSize, totalOutputSize, batch.size());

    // wake up the clients
    for (size_t i = 0, size = batch.size(); i != size; ++i)
      batch[i]->Signal();
  }
} catch (const Queue::InterruptedError &) {
  // it's ok
}
