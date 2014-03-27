#pragma once

#include "IProtocol.h"

/// Stateless request processor.
class IProcessor {
  public:
    /// Process a request.
    virtual void process(IProtocol & protocol) = 0;
};
