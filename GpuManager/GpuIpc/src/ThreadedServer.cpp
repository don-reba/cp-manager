#include "ThreadedServer.h"

#include "IConnector.h"
#include "IOException.h"
#include "IProcessor.h"
#include "IProtocol.h"
#include "ITransport.h"

#include <syslog.h>

#include <iostream>

#include <boost/thread/thread.hpp>

using namespace boost;
using namespace std;

//-----------------
// public interface
//-----------------

void ThreadedServer::serve(
    IConnector       & connector,
    ProtocolFactory    protocolFactory,
    const IProcessor & processor) {
  for (;;) {
    shared_ptr<ITransport> transport = connector.accept();
    shared_ptr<IProtocol>  protocol  = protocolFactory(*transport);
    thread(ConnectionHandler(transport, protocol, processor));
  }
}

//----------------------------------
// ThreadedServer::ConnectionHandler
//----------------------------------

ThreadedServer::ConnectionHandler::ConnectionHandler(
    shared_ptr<ITransport>   transport,
    shared_ptr<IProtocol>    protocol,
    const IProcessor       & processor) :

    m_transport (transport),
    m_protocol  (protocol),
    m_processor (processor) {
}

void ThreadedServer::ConnectionHandler::operator () ()
try {
  while (m_processor.process(*m_protocol)) {
  }
} catch (const IOException &) {
  // connection closed, no need for alarm
} catch (const std::exception & e) {
  cout << "Unrecoverable error: " << e.what() << '\n';
  syslog(LOG_ERR, "Unrecoverable error: %s", e.what());
} catch (...) {
  cout << "Unknown unrecoverable error." << '\n';
  syslog(LOG_ERR, "Unknown unrecoverable error.");
}
