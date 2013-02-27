#pragma once

#include <string>

#include "IConnector.h"

class SocketServerConnector : public IConnector {
  public: // public interface

    SocketServerConnector(const char * path);

    virtual ~SocketServerConnector();

  public: // IConnector implementation

    virtual boost::shared_ptr<ITransport> accept();

  private: // data

    std::string m_path;
    int         m_socket;
};
