#pragma once

#include "BlockingBatchQueue.h"
#include "DataPacket.h"
#include "GpuIpc/IProcessor.h"

#include "GpuHandler/IGpuHandler.h"

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

  typedef std::map<std::string, IGpuHandler*> HandlerMap;

  typedef BlockingBatchQueue<DataPacket*> Queue;

public: // interface

  MainServer(PerfLog & perfLog, DataLog & dataLog);
  ~MainServer();

  void start();
  void stop();

  void loadHandler(const std::string & handlerName);

public: // IProcess implementation

  virtual void process(IProtocol & protocol);

private: // private functions

  static size_t addSize(size_t total, const Data * data);

  static void * allocVector(
      std::size_t             index,
      std::size_t             size,
      IGpuHandler::AllocParam param);

  std::string createInvalidHandlerMsg(const std::string & handler) const;

  void processQueue();

private:

  HandlerMap m_handlers;

  PerfLog & m_perfLog;
  DataLog & m_dataLog;

  Queue m_dataQueue;

  boost::thread m_processingThread;
};
