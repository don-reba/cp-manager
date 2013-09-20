#pragma once

#include <cstddef>

class ITransport {
  public:
    virtual ~ITransport() {}

    virtual void readBytes(void * data, size_t size) = 0;

    virtual void writeBytes(const void * data, size_t size) = 0;
};
