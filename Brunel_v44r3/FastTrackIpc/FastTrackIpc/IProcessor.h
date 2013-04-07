#pragma once

#include "IProtocol.h"

// Stateless request processor.
class IProcessor {
  public:
    virtual bool process(IProtocol & protocol) const = 0;
};
