#pragma once

class ITransport {
  public:
    virtual ~ITransport() {}

    virtual void readBytes(char * data, int size) = 0;

    virtual void writeBytes(const char * data, int size) = 0;
};
