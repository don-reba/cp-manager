// Automatically generated file
#pragma once
#include <map>
#include <set>
#include <stdexcept>
#include <stdint.h>
#include <vector>
#include "GpuIpc/Api/Api.h"
#include "GpuIpc/IProcessor.h"
class IProtocol;
class AdminServer : public IProcessor {
public:
  // IProcess implementation
  virtual bool process(IProtocol & protocol);
private:
  // service functions for the user to implement
  void Exit();
private:
  // generated wrappers
  void process_Exit(IProtocol & protocol);
};
