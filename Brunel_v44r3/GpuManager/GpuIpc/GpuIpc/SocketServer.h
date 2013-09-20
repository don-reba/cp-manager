#pragma once

#include "ITransport.h"

#include <string>

class SocketServer : public ITransport {
  public: // public interface

    SocketServer(int m_socket);

    virtual ~SocketServer();

  public: // ITransport implementation

    virtual void readBytes(void * data, size_t size);

    virtual void writeBytes(const void * data, size_t size);

  private: // data

    int m_socket;
};
