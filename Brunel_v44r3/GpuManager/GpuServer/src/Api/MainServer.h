#pragma once
#include <map>
#include <string>
#include "GpuIpc/IProcessor.h"

class IProtocol;
class MainServer : public IProcessor {
private:

  typedef void * AllocParam;
  typedef void * (*Alloc)(size_t size, AllocParam param);

  typedef void (MainServer::*Handler)(void * data, size_t size, Alloc resultAlloc, AllocParam resultAllocParam);
  typedef std::map<std::string, Handler> HandlerMap;

public: // interface

  MainServer();

public: // IProcess implementation

  virtual void process(IProtocol & protocol);

private: // service functions

  static void * vectorAlloc(size_t size, AllocParam param);

private: // handlers

  void process_SearchByPair(
      void *     data,
      size_t     size,
      Alloc      allocResult,
      AllocParam allocResultParam);
  void process_Test(
      void *     data,
      size_t     size,
      Alloc      allocResult,
      AllocParam allocResultParam);

private:

  HandlerMap m_handlers;
};
