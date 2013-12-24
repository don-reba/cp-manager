#pragma once

#include "BlockingQueue.h"
#include "DataPacket.h"
#include "GpuIpc/IProcessor.h"
#include "PerfLog.h"

#include <map>
#include <stdint.h>
#include <string>
#include <vector>

#include <boost/thread/thread.hpp>

class IProtocol;
class MainServer : public IProcessor {
private:

  typedef void * AllocParam;
  typedef void * (*Alloc)(size_t size, AllocParam param);

  typedef void (MainServer::*Handler)(const Data & data, Alloc resultAlloc, AllocParam resultAllocParam);
  typedef std::map<std::string, Handler> HandlerMap;

public: // interface

  MainServer();

  void start();
  void stop();

public: // IProcess implementation

  virtual void process(IProtocol & protocol);

private: // private functions

  static void * allocVector(size_t size, AllocParam param);

  std::string createInvalidHandlerMsg(const std::string & handler) const;

  void processQueue();

private: // handlers

  void process_SearchByPair(
      const Data & data,
      Alloc        allocResult,
      AllocParam   allocResultParam);
  void process_Test(
      const Data & data,
      Alloc        allocResult,
      AllocParam   allocResultParam);

private:

  HandlerMap m_handlers;
  PerfLog    m_perfLog;

  BlockingQueue<DataPacket*>  m_dataQueue;

  boost::thread m_processingThread;
};
