#pragma once

#include <string>

class SocketClient {
  public:
    SocketClient(const char * path);
    ~SocketClient();

  private:
    std::string m_path;
    int         m_socket;
};
