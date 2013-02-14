#pragma once

#include "ITransport.h"

#include <string>

class SocketServer : public ITransport {
  public:
    SocketServer(const char * path);

    virtual ~SocketServer();

    void accept();

    // ITransport implementation

    virtual void readBytes(char * data, size_t size);

    virtual void writeBytes(const char * data, size_t size);

  private:
    std::string m_path;
    int         m_socket;
    int         m_accepted;
};
