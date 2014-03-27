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

  typedef std::map<std::string, Handlers::Handler> HandlerMap;

  typedef BlockingBatchQueue<DataPacket*> Queue;

public: // interface

  MainServer(PerfLog & perfLog, DataLog & dataLog);

  void start();
  void stop();

public: // IProcess implementation

  virtual void process(IProtocol & protocol);

private: // private functions

  static size_t addSize(size_t total, const Data * data);

  static void * allocVector(
      std::size_t          index,
      std::size_t          size,
      Handlers::AllocParam param);

  std::string createInvalidHandlerMsg(const std::string & handler) const;

  void processQueue();

private:

  HandlerMap m_handlers;

  PerfLog & m_perfLog;
  DataLog & m_dataLog;

  Queue m_dataQueue;

  boost::thread m_processingThread;
};
