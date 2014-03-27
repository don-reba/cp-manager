#include "IOException.h"
#include "SocketServer.h"
#include "SocketServerConnector.h"
#include "SystemException.h"

#include <boost/make_shared.hpp>
#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/un.h>

//-----------------
// public interface
//-----------------

SocketServerConnector::SocketServerConnector(const char * path) :
    m_path   (path),
    m_socket (-1) {
  m_socket = ::socket(AF_LOCAL, SOCK_STREAM, 0);
  if (m_socket == -1)
    throw SystemException("Could not create socket.");

  int opt = 1;
  ::setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  sockaddr_un address = { 0, { 0 } };
  address.sun_family = AF_UNIX;
  const size_t maxPathLength = sizeof(address.sun_path) - 1;
  if (m_path.size() > maxPathLength)
    throw std::runtime_error("The socket path is too long.");
  std::strncpy(address.sun_path, m_path.c_str(), maxPathLength);
  ::unlink(m_path.c_str());

  if (-1 == ::bind(m_socket, (sockaddr*)&address, SUN_LEN(&address)))
    throw SystemException("Could not bind to socket.");

  if (-1 == listen(m_socket, SOMAXCONN))
    throw SystemException("Could not listen to socket.");
}

SocketServerConnector::~SocketServerConnector() {
  if (m_socket != -1) {
    ::unlink(m_path.c_str());
    ::close(m_socket);
  }
}

//--------------------------
// IConnector implementation
//--------------------------

boost::shared_ptr<ITransport> SocketServerConnector::accept() {
  int accepted = ::accept(m_socket, NULL, NULL);
  if (-1 == accepted)
    throw IOException("Could not accept incoming connection.");
  return boost::make_shared<SocketServer>(accepted);
}

void SocketServerConnector::close() {
  shutdown(m_socket, SHUT_RDWR);
}
