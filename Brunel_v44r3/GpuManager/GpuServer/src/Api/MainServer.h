#pragma once
#include <map>
#include <string>
#include "GpuIpc/IProcessor.h"

class IProtocol;
class MainServer : public IProcessor {
private:

  typedef void (MainServer::*Handler)(void * data, size_t size);
  typedef std::map<std::string, Handler> HandlerMap;

public: // interface

  MainServer();

public: // IProcess implementation

  virtual void process(IProtocol & protocol);

private: // service functions for the user to implement

  void searchByPair(void * data, size_t size);

private: // handlers

  void process_searchByPair(void * data, size_t size);

private:

  HandlerMap m_handlers;
};
