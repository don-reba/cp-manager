#include "SocketServer.h"
#include "SystemException.h"

#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/un.h>

#include <sstream>

//-------------
// construction
//-------------

SocketServer::SocketServer(const char * path) :
    m_path     (path),
    m_socket   (-1),
    m_accepted (-1){
  m_socket = socket(AF_LOCAL, SOCK_STREAM, 0);
  if (m_socket == -1)
    throw SystemException("Could not create socket.");

  int opt = 1;
  setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  sockaddr_un address = { 0 };
  address.sun_family = AF_UNIX;
  const size_t maxPathLength = sizeof(address.sun_path) - 1;
  if (m_path.size() > maxPathLength)
    throw std::runtime_error("The socket path is too long.");
  std::strncpy(address.sun_path, m_path.c_str(), maxPathLength);
  unlink(m_path.c_str());

  if (-1 == bind(m_socket, (sockaddr*)&address, SUN_LEN(&address)))
    throw SystemException("Could not bind to socket.");

  if (-1 == listen(m_socket, SOMAXCONN))
    throw SystemException("Could not listen to socket.");
}

SocketServer::~SocketServer() {
  if (m_accepted != -1) {
    close(m_accepted);
  }
  if (m_socket != -1) {
    unlink(m_path.c_str());
    close(m_socket);
  }
}

void SocketServer::accept() {
  m_accepted = ::accept(m_socket, NULL, NULL);
	if (-1 == m_accepted)
		throw SystemException("Could not accept incoming connection.");
}

//--------------------------
// ITransport implementation
//--------------------------

void SocketServer::readBytes(char * data, size_t size) {
  if (-1 == m_accepted)
    throw std::runtime_error("Attempt to before accepting a connection.");

  size_t received = read(m_accepted, data, size);
  if (received == static_cast<size_t>(-1))
    throw SystemException("Read error.");
  if (received != size) {
    std::stringstream msg;
    msg << "Asked for " << size << " bytes; received " << received << ".";
    throw std::runtime_error(msg.str().c_str());
  }
}

void SocketServer::writeBytes(const char * data, size_t size) {
  if (-1 == m_accepted)
    throw std::runtime_error("Attempt to write before accepting a connection.");

  size_t sent = write(m_accepted, data, size);
  if (sent == static_cast<size_t>(-1))
    throw SystemException("Write error.");
  if (sent != size) {
    std::stringstream msg;
    msg << "Asked for " << size << " bytes; sent " << sent << ".";
    throw std::runtime_error(msg.str().c_str());
  }
}
