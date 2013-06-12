#include "App.h"

#include <cstdio>
#include <syslog.h>

#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <boost/ref.hpp>
#include <boost/thread.hpp>

using namespace boost;
using namespace std;

App::App(bool useStdIO) :
    m_useStdIO         (useStdIO),
    m_adminConnector   (makePath("admin").c_str()),
    m_trackerConnector (makePath("tracker").c_str()),
    m_adminServer      (m_adminConnector,   &App::getProtocol, m_admin),
    m_trackerServer    (m_trackerConnector, &App::getProtocol, m_tracker),
    m_admin            (*this) {
  if (!m_useStdIO) {
    freopen("/dev/null", "r", stdin);
    freopen("/dev/null", "w", stdout);
    freopen("/dev/null", "w", stderr);

    openlog("fasttrackd", LOG_CONS, LOG_DAEMON);
  }
}

App::~App() {
  if (m_useStdIO)
    closelog();
}

void App::run() {
  printMessage("starting");

	shared_ptr<thread> adminServer   (m_adminServer.serve());
	shared_ptr<thread> trackerServer (m_trackerServer.serve());

  printMessage("stopping");

  //exit();

  adminServer->join();
  trackerServer->join();
}

void App::exit() {
  m_adminServer.stop();
  m_trackerServer.stop();
}

void App::printError(const char * text) {
  if (m_useStdIO)
    fprintf(stderr, "%s\n", text);
  else
    syslog(LOG_ERR, "%s\n", text);
}

void App::printMessage(const char * text) {
  if (m_useStdIO)
    fprintf(stdout, "%s\n", text);
  else
    syslog(LOG_INFO, "%s\n", text);
}

shared_ptr<IProtocol> App::getProtocol(ITransport & transport) {
  return make_shared<Protocol>(ref(transport));
}

string App::makePath(const char * name) {
  return string("/tmp/GpuManager-") + name;
}
