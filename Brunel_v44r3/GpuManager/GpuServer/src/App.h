#pragma once

#include "IApp.h"

#include "Logger.h"

#include "Api/AdminServer.h"
#include "Api/TrackerServer.h"

#include "GpuIpc/Protocol.h"
#include "GpuIpc/SocketServerConnector.h"
#include "GpuIpc/ThreadedServer.h"

#include <boost/shared_ptr.hpp>

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
      const char * adminPath,
      const char * trackerPath);

    void run();

  public: // IApp implementation

    /// Safely terminates the application.
    virtual void exit();

  private: // private functions

    static boost::shared_ptr<IProtocol> getProtocol(ITransport & transport);

  private: // data

    Logger & m_logger;

    SocketServerConnector m_adminConnector;
    SocketServerConnector m_trackerConnector;

    ThreadedServer m_adminServer;
    ThreadedServer m_trackerServer;

    TrackerServer m_tracker;
    AdminServer   m_admin;
};
