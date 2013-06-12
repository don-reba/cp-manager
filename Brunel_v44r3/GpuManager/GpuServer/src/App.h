#pragma once

#include "IApp.h"

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
  public:
    /// @param useStdIO switches between the use of the standard streams and
    /// syslog.
    App(bool useStdIO);
    ~App();

    void run();

  public: // IApp implementation
    /// Safely terminates the application.
    virtual void exit();

    /// Prints an error message.
    /// @param text is the message to be printed.
    virtual void printError(const char * text);
    //
    /// Prints an information message.
    /// @param text is the message to be printed.
    virtual void printMessage(const char * text);

  private: // private functions
    static boost::shared_ptr<IProtocol> getProtocol(ITransport & transport);

    static std::string makePath(const char * name);

  private: // data
    const bool m_useStdIO;

    SocketServerConnector m_adminConnector;
    SocketServerConnector m_trackerConnector;

    ThreadedServer m_adminServer;
    ThreadedServer m_trackerServer;

    TrackerServer m_tracker;
    AdminServer   m_admin;
};
