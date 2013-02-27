#include "ThreadedServer.h"

#include "IConnector.h"
#include "IProcessor.h"
#include "IProtocol.h"
#include "ITransport.h"

#include <boost/thread/thread.hpp>

using namespace boost;

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

void ThreadedServer::ConnectionHandler::operator () () {
  while (m_processor.process(*m_protocol)) {
  }
}
