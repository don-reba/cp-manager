#pragma once

#include "IProtocol.h"

class ITransport;

class Protocol : public IProtocol {
  public: // public interface

    Protocol(ITransport & transport);

    virtual ~Protocol();

  public: // IProtocol implementation

    /// Reads an unsigned 32-bit integer.
    virtual uint32_t readUInt32();

    /// Writes an unsigned 32-bit integer.
    virtual void writeUInt32(uint32_t n);

    /// Reads a standard string.
    virtual std::string readString();

    /// Writes a standard string.
    virtual void writeString(const std::string & str);

    /// Reads a block of data.
    virtual void readData(void * data, size_t size);

    /// Writes a block of data.
    virtual void writeData(const void * data, size_t size);

  private:
    ITransport & m_transport;
};
