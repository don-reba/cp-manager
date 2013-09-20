#pragma once
#include <map>
#include <string>
#include "GpuIpc/IProcessor.h"

class IProtocol;
class TrackerServer : public IProcessor {
private:

  typedef void (TrackerServer::*Handler)(void * data, size_t size);
  typedef map<std::string, Handler> HandlerMap;

public: // interface

  MainServer();

public: // IProcess implementation

  virtual void process(IProtocol & protocol);

private: // service functions for the user to implement

  void searchByPair(void * data, size_t size);

private: // handlers

  void process_searchByPair(IProtocol & protocol);

private:

  HandlerMap m_handlers;
};
