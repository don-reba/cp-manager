#pragma once

#include "ITransport.h"

#include <boost/shared_ptr.hpp>

/// Helper interface used by the server to handle incoming connections.
class IConnector {
  public:
    virtual ~IConnector() {}

    /// Block until a client connects or throw an exception.
    virtual boost::shared_ptr<ITransport> accept() = 0;

    /// Called when the server is halted.
    virtual void close() = 0;
};
