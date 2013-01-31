#include "SocketServer.h"
#include "SystemException.h"

#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/un.h>

SocketServer::SocketServer(const char * path) :
    m_path   (path),
    m_socket (-1) {
  const size_t maxPathLength = 107;
  if (m_path.size() > maxPathLength)
    throw std::runtime_error("The socket path is too long.");

  m_socket = socket(AF_LOCAL, SOCK_STREAM, 0);
  if (m_socket == -1)
    throw SystemException("Could not create socket.");

  int opt = 1;
  setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  sockaddr_un address;
  address.sun_family = AF_LOCAL;
  std::strcpy(address.sun_path, m_path.c_str());
  unlink(m_path.c_str());

  if (-1 == bind(m_socket, (sockaddr*)&address, sizeof(address)))
    throw SystemException("Could not bind to socket.");

  if (-1 == listen(m_socket, SOMAXCONN))
    throw SystemException("Could not listen to socket.");
}

SocketServer::~SocketServer() {
  if (m_socket != -1) {
    unlink(m_path.c_str());
    close(m_socket);
  }
}

void SocketServer::accept() {
	if (-1 == ::accept(m_socket, NULL, NULL))
		throw SystemException("Could not accept incoming connection.");
}

void SocketServer::readBytes(char * data, int size) {
}

void SocketServer::writeBytes(const char * data, int size) {
}
