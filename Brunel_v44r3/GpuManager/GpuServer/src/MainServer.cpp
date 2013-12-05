#include "MainServer.h"
#include "GpuIpc/IProtocol.h"
#include "Timer.h"

#include "PatPixelSerialization/Serialization.h"
#include "PixelTracker/PixelImplementation.h"

#include <algorithm>
#include <cstring>
#include <ctime>
#include <stdexcept>
#include <iostream>
#include <sstream>

using namespace std;

//----------
// interface
//----------

MainServer::MainServer() :
    m_perfLog("perf.log") {
  m_handlers["searchByPair"] = &MainServer::process_SearchByPair;
  m_handlers["test"]         = &MainServer::process_Test;
}

//------------------------
// IProcess implementation
//------------------------

void MainServer::process(IProtocol & protocol) {
  const size_t FAIL_FLAG = 0xFFFFFFFF;

  std::string handlerName = protocol.readString();

  bool isProfiling = protocol.readBool();
  Timer timer(isProfiling);

  size_t size = protocol.readUInt32();

  vector<uint8_t> data(size);
  protocol.readData(&data[0], size);

  HandlerMap::const_iterator i = m_handlers.find(handlerName);
  if (i == m_handlers.end()) {
    // when a handler is not found, inform the client
    protocol.writeUInt32(FAIL_FLAG);
    protocol.writeString(createInvalidHandlerMsg(handlerName));
    return;
  }

  // call the handler
  Data result;
  Handler handler = i->second;
  try {
    timer.start();
    (this->*handler)(data, vectorAlloc, &result);
    timer.stop();
  } catch (const exception & e) {
    // when the handler crashes, inform the client
    protocol.writeUInt32(FAIL_FLAG);
    protocol.writeString(e.what());
    return;
  }

  // return the result to client
  protocol.writeUInt32(result.size());
  protocol.writeData(&result[0], result.size());

  if (isProfiling) {
    double totalSeconds  = protocol.readDouble();
    double kernelSeconds = timer.secondsElapsed();
    m_perfLog.addRecord(time(0), handlerName.c_str(), totalSeconds, kernelSeconds, size, result.size());
  }
}

//------------------
// Service functions
//------------------

void * MainServer::vectorAlloc(size_t size, AllocParam param) {
  typedef vector<uint8_t> Data;
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

//---------
// handlers
//---------

void MainServer::process_SearchByPair(
    const Data & data,
    Alloc        allocResult,
    AllocParam   allocResultParam) {
  cout << "'searchByPair' received " << data.size() << " bytes" << endl;

  PixelEvent event;
  deserializePixelEvent(data, event);

  std::vector<GpuTrack> tracks;
  pixel_tracker_implementation(event, tracks);

  // TODO: move allocation out of serialization
  Data result;
  serializeGpuTracks(tracks, result);

  void * buffer = allocResult(result.size(), allocResultParam);
  copy(result.begin(), result.end(), (uint8_t*)buffer);
}

void MainServer::process_Test(
    const Data & data,
    Alloc        allocResult,
    AllocParam   allocResultParam) {
  cout << "'test' received " << data.size() << " bytes" << endl;

  static uint32_t callCount = 0;
  *(uint32_t*)allocResult(4, allocResultParam) = callCount;
  ++callCount;
}
