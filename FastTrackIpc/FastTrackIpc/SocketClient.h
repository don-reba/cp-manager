#pragma once

#include "ITransport.h"

#include <string>

class SocketClient : public ITransport {
  public:
    SocketClient(const char * path);

    virtual ~SocketClient();

    // ITransport implementation

    virtual void readBytes(char * data, size_t size);

    virtual void writeBytes(const char * data, size_t size);

  private:
    std::string m_path;
    int         m_socket;
};
