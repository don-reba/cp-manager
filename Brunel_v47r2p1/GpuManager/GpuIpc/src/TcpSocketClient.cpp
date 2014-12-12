#include "TcpSocketClient.h"
#include "SystemException.h"

#include <arpa/inet.h>
#include <cstring>
#include <netinet/in.h>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>

#include <sstream>

//-------------
// construction
//-------------

TcpSocketClient::TcpSocketClient(int port, const std::string& hname) :
    m_host   (hname),
    m_socket (-1),
    m_port   (port) {
  m_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (m_socket == -1)
    throw SystemException("Could not create socket.");

  sockaddr_in address = {};
  address.sin_family      = AF_INET;
  address.sin_port        = htons(m_port);
  address.sin_addr.s_addr = inet_addr(m_host.c_str());

  if (-1 == connect(m_socket, (sockaddr*)&address, sizeof(address)))
    throw SystemException("Could not connect to socket.");
}

TcpSocketClient::~TcpSocketClient() {
  if (m_socket != -1)
    close(m_socket);
}

//--------------------------
// ITransport implementation
//--------------------------

void TcpSocketClient::readBytes(void * data, size_t size) {
  size_t total = 0u;
  while (total < size) {
    size_t received = read(m_socket, static_cast<uint8_t*>(data) + total, size - total);
    if (received == static_cast<size_t>(-1)) {
      std::stringstream msg;
      msg << "Read error; " << total << " bytes received.";
      throw std::runtime_error(msg.str().c_str());
    }
    total += received;
  }
  if (total > size) {
    std::stringstream msg;
    msg << "Asked for " << size << " bytes; received " << total << ".";
    throw std::runtime_error(msg.str().c_str());
  }
}

void TcpSocketClient::writeBytes(const void * data, size_t size) {
  size_t sent = write(m_socket, data, size);
  if (sent == static_cast<size_t>(-1))
    throw SystemException("Write error.");
  if (sent != size) {
    std::stringstream msg;
    msg << "Asked for " << size << " bytes; sent " << sent << ".";
    throw std::runtime_error(msg.str().c_str());
  }
}
