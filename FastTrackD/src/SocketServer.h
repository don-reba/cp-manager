#pragma once

#include "ITransport.h"

#include <string>

class SocketServer : public ITransport {
  public:
    SocketServer(const char * path);

    virtual ~SocketServer();

    void accept();

    // ITransport implementation

    virtual void readBytes(char * data, int size);

    virtual void writeBytes(const char * data, int size);

  private:
    std::string m_path;
    int         m_socket;
};
