/*
 * Tcp socket server adapted from the LocalSocketServerConnector
 * by Alexey Badalov
 */

#include "IOException.h"
#include "SocketServer.h"
#include "TcpSocketServerConnector.h"
#include "SystemException.h"

#include <cstring>
#include <stdexcept>
#include <sys/socket.h>
#include <sys/un.h>
#include <iostream>
// Marco 23-07-2014: added to bind the socket to a real IP address
#include <sys/types.h>
#include <ifaddrs.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>

//-----------------
// public interface
//-----------------

TcpSocketServerConnector::TcpSocketServerConnector(int port, const std::string& hname) :
    m_port   (port),
    m_host   (hname),
    m_socket (-1) {
  m_socket = ::socket(AF_INET, SOCK_STREAM, 0);
  if (m_socket == -1)
    throw SystemException("Could not create socket.");

  //int opt = 1;
  //::setsockopt(m_socket, SOL_SOCKET, SO_REUSEADDR, &opt, sizeof(opt));

  struct sockaddr_in serv_addr;
  bzero((char *) &serv_addr, sizeof(serv_addr));

  serv_addr.sin_family = AF_INET;
  if (m_host.compare("localhost") == 0)
      serv_addr.sin_addr.s_addr = INADDR_ANY;
  else
      serv_addr.sin_addr.s_addr = inet_addr(m_host.c_str());
  serv_addr.sin_port = htons(m_port);
  std::cout << "host and port: " << m_host << " and " << m_port << std::endl;
  if (::bind(m_socket, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
      throw SystemException("Could not bind to socket");
  }
  if (::listen(m_socket,5) < 0) {
      throw SystemException("Could not listen to socket");
  }

}

TcpSocketServerConnector::~TcpSocketServerConnector() {
  if (m_socket != -1) {
    ::close(m_socket);
  }
}

//--------------------------
// IConnector implementation
//--------------------------

std::shared_ptr<ITransport> TcpSocketServerConnector::accept() {
  int accepted = ::accept(m_socket, NULL, NULL);
  if (-1 == accepted)
    throw IOException("Could not accept incoming connection.");
  return std::make_shared<SocketServer>(accepted);
}

void TcpSocketServerConnector::close() {
  shutdown(m_socket, SHUT_RDWR);
}
