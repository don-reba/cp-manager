#pragma once

#include <string>

class SocketClient {
  public:
    SocketClient(const char * path);
    ~SocketClient();

  private:
    int         m_socket;
    std::string m_path;
};
