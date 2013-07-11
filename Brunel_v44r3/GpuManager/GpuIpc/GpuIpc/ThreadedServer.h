#pragma once

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/thread/thread.hpp>

class IConnector;
class IProcessor;
class IProtocol;
class ITransport;

class ThreadedServer {
  private: // helper tyeps

    class ConnectionHandler {
      public:
        ConnectionHandler (
          ThreadedServer                & server,
          boost::shared_ptr<ITransport>   transport,
          boost::shared_ptr<IProtocol>    protocol,
          IProcessor                    & processor);
        void operator () ();
      private:
        ThreadedServer                & m_server;
        boost::shared_ptr<ITransport>   m_transport;
        boost::shared_ptr<IProtocol>    m_protocol;
        IProcessor                    & m_processor;
    };

  public: // public interface

    typedef boost::function<boost::shared_ptr<IProtocol> (ITransport&)> ProtocolFactory;

    ThreadedServer(
      IConnector      & connector,
      ProtocolFactory   protocolFactory,
      IProcessor      & processor);

    boost::thread * serve();

    void stop();

  private: // private functions

    void serveConnector();

    bool stopRequested();

  private: // data

    IConnector      & m_connector;
    ProtocolFactory   m_protocolFactory;
    IProcessor      & m_processor;

    boost::thread_group m_connections;
};
