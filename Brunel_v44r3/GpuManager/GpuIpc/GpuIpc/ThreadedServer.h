#pragma once

#include <boost/function.hpp>
#include <boost/shared_ptr.hpp>

class IConnector;
class IProcessor;
class IProtocol;
class ITransport;

class ThreadedServer {
  private: // helper tyeps

    class ConnectionHandler
    {
      public:
        ConnectionHandler (
          boost::shared_ptr<ITransport>   transport,
          boost::shared_ptr<IProtocol>    protocol,
          const IProcessor              & processor);
        void operator () ();
      private:
          boost::shared_ptr<ITransport>   m_transport;
          boost::shared_ptr<IProtocol>    m_protocol;
          IProcessor                    & m_processor;
    };

  public: // public interface

    typedef boost::function<boost::shared_ptr<IProtocol> (ITransport&)> ProtocolFactory;

    void serve(
        IConnector      & connector,
        ProtocolFactory   protocolFactory,
        IProcessor      & processor);
};
