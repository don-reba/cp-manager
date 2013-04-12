#pragma once

#include "ITransport.h"

#include <boost/shared_ptr.hpp>

class IConnector {
  public:
    virtual ~IConnector() {}

    virtual boost::shared_ptr<ITransport> accept() = 0;
};
