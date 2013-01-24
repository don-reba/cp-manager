#include "SocketServer.h"

#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/un.h>

SocketServer::SocketServer(const char * path) :
    m_path   (path),
    m_socket (-1) {
  const size_t maxPathLength = 14;
  if (m_path.size() > maxPathLength)
    throw std::runtime_error("Socket path can be at most 14 symbols long.");

  m_socket = socket(AF_LOCAL, SOCK_STREAM, 0);
  if (m_socket == -1)
    throw std::runtime_error("Could not create socket.");

  int opt = 1;
  setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  sockaddr_un address;
  address.sun_family = AF_LOCAL;
  std::strncpy(address.sun_path, m_path.c_str(), maxPathLength);
  unlink(m_path.c_str());

  if (-1 == bind(m_socket, (sockaddr*)&address, sizeof(address)))
    throw std::runtime_error("Could not bind to socket.");

  if (-1 == listen(m_socket, SOMAXCONN))
    throw std::runtime_error("Could not listen to socket.");
}

SocketServer::~SocketServer() {
  if (m_socket != -1) {
    unlink(m_path.c_str());
    close(m_socket);
  }
}

void SocketServer::accept() {
	if (-1 == ::accept(m_socket, NULL, NULL))
		throw std::runtime_error("Could not accept incoming connection.");
}

void SocketServer::readBytes(char * data, int size) {
}

void SocketServer::writeBytes(const char * data, int size) {
}
