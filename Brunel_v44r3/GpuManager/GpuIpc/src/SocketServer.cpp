#include "SocketServer.h"

#include "IOException.h"
#include "SystemException.h"

#include <cstring>
#include <sys/socket.h>
#include <sys/un.h>

#include <sstream>

//-----------------
// public interface
//-----------------

SocketServer::SocketServer(int socket) :
    m_socket (socket) {
}

SocketServer::~SocketServer() {
  if (m_socket != -1) 
    close(m_socket);
}

//--------------------------
// ITransport implementation
//--------------------------

void SocketServer::readBytes(void * data, size_t size) {
  size_t received = read(m_socket, data, size);
  if (received == static_cast<size_t>(-1))
    throw SystemException("Read error.");
  if (received != size) {
    std::stringstream msg;
    msg << "Asked for " << size << " bytes; received " << received << ".";
    throw IOException(msg.str());
  }
}

void SocketServer::writeBytes(const void * data, size_t size) {
  size_t sent = write(m_socket, data, size);
  if (sent == static_cast<size_t>(-1))
    throw SystemException("Write error.");
  if (sent != size) {
    std::stringstream msg;
    msg << "Asked for " << size << " bytes; sent " << sent << ".";
    throw IOException(msg.str());
  }
}
