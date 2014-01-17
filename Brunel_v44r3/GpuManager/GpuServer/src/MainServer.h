#pragma once

#include "BlockingBatchQueue.h"
#include "DataPacket.h"
#include "GpuIpc/IProcessor.h"
#include "Handlers/Handler.h"

#include <map>
#include <stdint.h>
#include <string>
#include <vector>

#include <boost/thread/thread.hpp>

class IProtocol;
class DataLog;
class PerfLog;

class MainServer : public IProcessor {
private:

  typedef std::map<std::string, Handler> HandlerMap;

  typedef BlockingBatchQueue<DataPacket*> Queue;

public: // interface

  MainServer(PerfLog & perfLog, DataLog & dataLog);

  void start();
  void stop();

public: // IProcess implementation

  virtual void process(IProtocol & protocol);

private: // private functions

  static void * allocVector(size_t index, size_t size, AllocParam param);

  std::string createInvalidHandlerMsg(const std::string & handler) const;

  void processQueue();

private:

  HandlerMap m_handlers;

  PerfLog & m_perfLog;
  DataLog & m_dataLog;

  Queue m_dataQueue;

  boost::thread m_processingThread;
};
