#pragma once

#include "Logger.h"

#include "Admin.h"

#include "GpuIpc/SocketClient.h"
#include "GpuIpc/Protocol.h"

class Controller {
  public: // interface

    Controller(
      Logger     & logger,
      const char * adminPath);

    void stopServer();

  public: // data

    Logger & m_logger;

    SocketClient m_transport;
    Protocol     m_protocol;
    Admin        m_admin;
};
