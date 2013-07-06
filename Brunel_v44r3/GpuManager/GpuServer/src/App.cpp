#include "App.h"

#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <boost/ref.hpp>
#include <boost/thread.hpp>

using namespace boost;
using namespace std;

App::App(
    Logger     & logger,
    const char * adminPath,
    const char * trackerPath) :
    // initializers
    m_logger           (logger),
    m_adminConnector   (adminPath),
    m_trackerConnector (trackerPath),
    m_adminServer      (m_adminConnector,   &App::getProtocol, m_admin),
    m_trackerServer    (m_trackerConnector, &App::getProtocol, m_tracker),
    m_admin            (*this) {
}

void App::run() {
	shared_ptr<thread> adminServer   (m_adminServer.serve());
	shared_ptr<thread> trackerServer (m_trackerServer.serve());

  adminServer->join();
  trackerServer->join();
}

void App::exit() {
  m_adminServer.stop();
  m_trackerServer.stop();
}

shared_ptr<IProtocol> App::getProtocol(ITransport & transport) {
  return make_shared<Protocol>(ref(transport));
}
