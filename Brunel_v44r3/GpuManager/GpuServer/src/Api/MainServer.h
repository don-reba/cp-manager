#pragma once

#include "GpuIpc/IProcessor.h"
#include "PerfLog.h"

#include <map>
#include <stdint.h>
#include <string>
#include <vector>

class IProtocol;
class MainServer : public IProcessor {
private:

  typedef std::vector<uint8_t> Data;

  typedef void * AllocParam;
  typedef void * (*Alloc)(size_t size, AllocParam param);

  typedef void (MainServer::*Handler)(const Data & data, Alloc resultAlloc, AllocParam resultAllocParam);
  typedef std::map<std::string, Handler> HandlerMap;

public: // interface

  MainServer();

public: // IProcess implementation

  virtual void process(IProtocol & protocol);

private: // service functions

  static void * vectorAlloc(size_t size, AllocParam param);

  std::string createInvalidHandlerMsg(const std::string & handler) const;

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
};
