#pragma once

#include "IApp.h"

#include "Logger.h"

#include "AdminServer.h"
#include "MainServer.h"

#include "GpuIpc/Protocol.h"
#include "GpuIpc/TcpSocketServerConnector.h"
#include "GpuIpc/LocalSocketServerConnector.h"
#include "GpuIpc/ThreadedServer.h"

#include <memory>

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
      const char * mainPath,
      const std::string& host,
      const int    port,
      const std::string& connector);

    void run();

  public: // IApp implementation

    /// Safely terminates the application.
    virtual void exit();

    /// Load a GPU handler component.
    virtual void loadHandler(const std::string & handlerHanel);

  private: // private functions

    static std::shared_ptr<IProtocol> getProtocol(ITransport & transport);

  private: // data

    Logger  & m_logger;

    // Marco 23-07-2014: Substitute LocalSocket with a pointer to IConnector
    //                   to let App use different Connectors, not only Local ones
    
    LocalSocketServerConnector m_adminConnector;
    //IConnector* m_mainConnector;
    std::shared_ptr<IConnector> m_mainConnector;

    //std::shared_ptr<IConnector> m_adminConnector;
    //std::shared_ptr<IConnector> m_mainConnector;
    ThreadedServer m_adminServer;
    ThreadedServer m_mainServer;

    MainServer  m_main;
    AdminServer m_admin;
};
