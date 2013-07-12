#pragma once
#include <map>
#include <set>
#include <stdexcept>
#include <stdint.h>
#include <vector>
#include "GpuIpc/Api/Api.h"
#include "GpuIpc/IProcessor.h"

class IApp;
class IProtocol;

class AdminServer : public IProcessor {
  public: // interface
    AdminServer(IApp & app);

  public:
    // IProcess implementation
    virtual void process(IProtocol & protocol);

  private:
    void Exit();

  private: // generated wrappers
    void process_Exit(IProtocol & protocol);

  private:
    IApp & m_app;
};
