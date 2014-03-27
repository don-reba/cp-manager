#include "App.h"

#include <boost/bind.hpp>
#include <boost/make_shared.hpp>
#include <boost/ref.hpp>
#include <boost/thread.hpp>

using namespace std;

App::App(
    Logger     & logger,
    PerfLog    & perfLog,
    DataLog    & dataLog,
    const char * adminPath,
    const char * mainPath) :
    // initializers
    m_logger         (logger),
    m_adminConnector (adminPath),
    m_mainConnector  (mainPath),
    m_adminServer    (m_adminConnector, &App::getProtocol, m_admin),
    m_mainServer     (m_mainConnector, &App::getProtocol, m_main),
    m_main           (perfLog, dataLog),
    m_admin          (*this) {
}

void App::run() {
  boost::shared_ptr<boost::thread> adminServer(m_adminServer.serve());
  boost::shared_ptr<boost::thread> mainServer(m_mainServer.serve());

  m_main.start();

  adminServer->join();
  mainServer->join();
}

void App::exit() {
  m_adminServer.stop();
  m_mainServer.stop();
  m_main.stop();
}

boost::shared_ptr<IProtocol> App::getProtocol(ITransport & transport) {
  return boost::make_shared<Protocol>(ref(transport));
}
