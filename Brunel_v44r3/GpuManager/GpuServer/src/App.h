#pragma once

#include "IApp.h"

#include "Logger.h"

#include "AdminServer.h"
#include "MainServer.h"

#include "GpuIpc/Protocol.h"
#include "GpuIpc/SocketServerConnector.h"
#include "GpuIpc/ThreadedServer.h"

#include <boost/shared_ptr.hpp>

class DataLog;
class PerfLog;

/** @class App App.h GpuServer/App.h
 * Application-wide API.
 *
 * @author Alexey Badalov
 * @date   2013-01-11
 */
class App : public IApp {
  public: // interface

    /// @param useStdIO switches between the use of the standard streams and
    /// syslog.
    App(
      Logger     & logger,
      PerfLog    & perfLog,
      DataLog    & dataLog,
      const char * adminPath,
      const char * mainPath);

    void run();

  public: // IApp implementation

    /// Safely terminates the application.
    virtual void exit();

  private: // private functions

    static boost::shared_ptr<IProtocol> getProtocol(ITransport & transport);

  private: // data

    Logger  & m_logger;

    SocketServerConnector m_adminConnector;
    SocketServerConnector m_mainConnector;

    ThreadedServer m_adminServer;
    ThreadedServer m_mainServer;

    MainServer  m_main;
    AdminServer m_admin;
};
