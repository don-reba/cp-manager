#pragma once

#include "IBlockingQueue.h"
#include "DataPacket.h"
#include "GpuIpc/IProcessor.h"

#include "GpuHandler/IGpuHandler.h"

#include <map>
#include <memory>
#include <stdint.h>
#include <string>
#include <vector>

#include <boost/thread/locks.hpp>
#include <boost/thread/mutex.hpp>
#include <boost/thread/thread.hpp>

class IProtocol;
class DataLog;
class PerfLog;

class MainServer : public IProcessor {
private:

  typedef std::map<std::string, IGpuHandler*> HandlerMap;

  typedef IBlockingQueue<DataPacket*> IQueue;

public: // interface

  MainServer(PerfLog & perfLog, DataLog & dataLog, size_t batchSize);
  ~MainServer();

  void start();
  void stop();

  void loadHandler(const std::string & handlerName);

public: // IProcess implementation

  virtual void process(IProtocol & protocol);

private: // private functions

  static size_t addSize(size_t total, const Data * data);

  static uint8_t * allocVector(
      std::size_t             index,
      std::size_t             size,
      IGpuHandler::AllocParam param);

  std::string createInvalidHandlerMsg(const std::string & handler) const;

  IGpuHandler * getHandlerByName(const std::string & name);

  void processQueue();

private:

  HandlerMap m_handlers;

  PerfLog & m_perfLog;
  DataLog & m_dataLog;

  std::unique_ptr<IQueue> m_dataQueue;

  boost::thread m_processingThread;

  typedef boost::mutex::scoped_lock scoped_lock;
  boost::mutex m_mutex;
};
