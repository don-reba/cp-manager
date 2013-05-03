#pragma once

#include <cstddef>

class ITransport {
  public:
    virtual ~ITransport() {}

    virtual void readBytes(char * data, size_t size) = 0;

    virtual void writeBytes(const char * data, size_t size) = 0;
};
