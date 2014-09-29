#include "TcpSocketClient.h"
#include "SystemException.h"

#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/un.h>
#include <unistd.h>
// Marco 25-07-2014: header added to implement TCP socket client
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

#include <sstream>

//-------------
// construction
//-------------

TcpSocketClient::TcpSocketClient(int port, const std::string& hname) :
    m_port(port),
    m_host(hname),
    m_socket (-1) {
  m_socket = socket(AF_INET, SOCK_STREAM, 0);
  if (m_socket == -1)
    throw SystemException("Could not create socket.");

  struct sockaddr_in address;
  bzero((char *) &address, sizeof(address));
  address.sin_family = AF_INET;
  address.sin_port = htons(m_port);
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
    size_t received = read(m_socket, data, size);
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
