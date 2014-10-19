#include "App.h"

#include <memory>

#include <boost/bind.hpp>
#include <boost/ref.hpp>
#include <boost/thread.hpp>

using namespace std;

App::App(
    Logger     & logger,
    PerfLog    & perfLog,
    DataLog    & dataLog,
		const char * adminPath,
    const ConnectionInfo & connection) :
    // initializers
    m_logger         (logger),
    m_adminConnector (adminPath),
    m_mainConnector  (createConnector(connection)),
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

shared_ptr<IConnector> App::createConnector(const ConnectionInfo & connection) {
	switch (connection.type()) {
		case ConnectionInfo::Local: {
			auto info = reinterpret_cast<const LocalConnectionInfo &>(connection);
			return make_shared<LocalSocketServerConnector>(info.path());
		}
		case ConnectionInfo::Tcp: {
			auto info = reinterpret_cast<const TcpConnectionInfo &>(connection);
			return make_shared<TcpSocketServerConnector>(info.port(), info.host());
		}
	}
	throw runtime_error("Unexpected connection type.");
}
