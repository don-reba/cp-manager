#include "App.h"

#include <memory>

#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/thread.hpp>

using namespace std;

/*
 * TODO: connector is now a simple "placeholder", must be used to select
 *       the specified socket server (i.e. tcp, local...)
 */

App::App(
    Logger     & logger,
    PerfLog    & perfLog,
    DataLog    & dataLog,
    const char * adminPath,
    const char * mainPath,
    const std::string& host,
    const int    port,
    const std::string& connector) :
    // initializers
    m_logger         (logger),
    m_adminConnector (adminPath),
    //m_mainConnector  (new TcpSocketServerConnector(65000, "131.154.184.23")),
    m_mainConnector  (new TcpSocketServerConnector(port, host)),
    m_adminServer    (m_adminConnector, &App::getProtocol, m_admin),
    m_mainServer     (*m_mainConnector, &App::getProtocol, m_main),
    m_main           (perfLog, dataLog),
    m_admin          (*this) {
}

void App::run() {
  shared_ptr<boost::thread> adminServer(m_adminServer.serve());
  shared_ptr<boost::thread> mainServer(m_mainServer.serve());

  m_main.start();

  adminServer->join();
  mainServer->join();
}

void App::exit() {
  try {
    m_adminServer.stop();
  } catch (const std::exception & e) {
    m_logger.printError(e.what());
  }

  try {
  m_mainServer.stop();
  } catch (const std::exception & e) {
    m_logger.printError(e.what());
  }

  try {
  m_main.stop();
  } catch (const std::exception & e) {
    m_logger.printError(e.what());
  }
}

void App::loadHandler(const string & handlerName) {
  try {
    m_main.loadHandler(handlerName);
  } catch (const std::exception & e) {
    m_logger.printError(e.what());
  }
}

shared_ptr<IProtocol> App::getProtocol(ITransport & transport) {
  return make_shared<Protocol>(ref(transport));
}
