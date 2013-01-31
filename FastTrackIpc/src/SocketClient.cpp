#include "SocketClient.h"
#include "SystemException.h"

#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/un.h>

#include <iostream>

SocketClient::SocketClient(const char * path) :
    m_path   (path),
    m_socket (-1) {
  const size_t maxPathLength = 107;
  if (m_path.size() > maxPathLength)
    throw std::runtime_error("The socket path is too long.");

  m_socket = socket(AF_LOCAL, SOCK_STREAM, 0);
  if (m_socket == -1)
    throw SystemException("Could not create socket.");

  sockaddr_un address;
  address.sun_family = AF_LOCAL;
  std::strcpy(address.sun_path, m_path.c_str());
  std::cout << address.sun_path << std::endl;
  if (-1 == connect(m_socket, (sockaddr*)&address, sizeof(address)))
    throw SystemException("Could not connect to socket.");
}

SocketClient::~SocketClient() {
  if (m_socket != -1)
    close(m_socket);
}
