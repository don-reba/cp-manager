#include "ThreadedServer.h"

#include "IConnector.h"
#include "IOException.h"
#include "IProcessor.h"
#include "IProtocol.h"
#include "ITransport.h"

#include <syslog.h>

#include <iostream>

using namespace boost;
using namespace std;

//-----------------
// public interface
//-----------------

ThreadedServer::ThreadedServer(
    IConnector      & connector,
    ProtocolFactory   protocolFactory,
    IProcessor      & processor) :
    // initializers
    m_connector       (connector),
    m_protocolFactory (protocolFactory),
    m_processor       (processor) {
}

thread * ThreadedServer::serve() {
  return new thread(&ThreadedServer::serveConnector, this);
}

void ThreadedServer::stop() {
  m_connector.close();
}

//------------------
// private functions
//------------------

void ThreadedServer::serveConnector() {
  try {
    for (;;) { // until an exception is thrown
      shared_ptr<ITransport> transport = m_connector.accept();
      shared_ptr<IProtocol>  protocol  = m_protocolFactory(*transport);
      // TODO: create a new processor for each thread
      m_connections.create_thread(ConnectionHandler(*this, transport, protocol, m_processor));
    }
  } catch (const IOException &) {
    // connection closed, no need for alarm
  }
  m_connections.join_all();
}

//----------------------------------
// ThreadedServer::ConnectionHandler
//----------------------------------

ThreadedServer::ConnectionHandler::ConnectionHandler(
    ThreadedServer         & server,
    shared_ptr<ITransport>   transport,
    shared_ptr<IProtocol>    protocol,
    IProcessor             & processor) :
    // initializers
    m_server    (server),
    m_transport (transport),
    m_protocol  (protocol),
    m_processor (processor) {
}

void ThreadedServer::ConnectionHandler::operator () ()
try {
  for (;;) // until an exception is thrown
    m_processor.process(*m_protocol);
} catch (const IOException &) {
  // connection closed, no need for alarm
} catch (const std::exception & e) {
  cout << "Connection error: " << e.what() << '\n';
  syslog(LOG_ERR, "Connection error: %s", e.what());
} catch (...) {
  cout << "Unknown connection error." << '\n';
  syslog(LOG_ERR, "Unknown connection error.");
}
