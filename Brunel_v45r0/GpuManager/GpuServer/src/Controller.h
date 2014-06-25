#pragma once

#include "Admin.h"
#include "Logger.h"

#include "GpuIpc/LocalSocketClient.h"
#include "GpuIpc/Protocol.h"

class Controller {
  public: // interface

    Controller(Logger & logger, const char * adminPath);

    void stopServer();
		void loadHandler(const std::string & handlerName);

  public: // data

    Logger & m_logger;

    LocalSocketClient m_transport;
    Protocol          m_protocol;
    Admin             m_admin;
};
