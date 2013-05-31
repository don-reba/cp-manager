// Automatically generated file
#pragma once
#include "GpuIpc/Api/Api.h"
#include <map>
#include <set>
#include <stdexcept>
#include <stdint.h>
#include <vector>
class IProtocol;
class Admin {
public:
  // interface
  Admin(IProtocol & protocol);
public:
  // service function wrappers
  void Exit(, void & result);
private:
  // data
  IProtocol & protocol;
};
