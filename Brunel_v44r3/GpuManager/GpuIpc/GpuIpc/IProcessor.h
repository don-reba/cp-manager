#pragma once

#include "IProtocol.h"

// Stateless request processor.
class IProcessor {
  public:
    virtual void process(IProtocol & protocol) = 0;
};
