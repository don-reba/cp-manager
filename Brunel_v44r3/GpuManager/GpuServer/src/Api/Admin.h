#pragma once

class IProtocol;
class Admin {
public:
  // interface
  Admin(IProtocol & protocol);
public:
  // service function wrappers
  void Exit();
private:
  // data
  IProtocol & protocol;
};
