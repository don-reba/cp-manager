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
  ssize_t total = 0;
  while (total < size) {
    ssize_t received = read(m_socket, data + total, size - total);
    if (received == 0)
      throw IOException("Received 0 bytes.");
    if (received == -1)
      throw SystemException("Read error.");
    total += received;
  }
}

void SocketServer::writeBytes(const void * data, size_t size) {
  ssize_t total = 0;
  while (total < size) {
    ssize_t sent = write(m_socket, data + total, size - total);
    if (sent == -1)
      throw SystemException("Write error.");
    total += sent;
  }
}
