#pragma once

#include <stdint.h>
#include <string>
#include <vector>

// The communication protocol.

/** @class IProtocol IProtocol.h GpuIpc/IProtocol.h
 *  Service interface encapsulating the message protocol.
 */
class IProtocol {
  public:
    virtual ~IProtocol() {}

    /// Reads an unsigned 32-bit integer.
    virtual uint32_t readUInt32() = 0;

    /// Writes an unsigned 32-bit integer.
    virtual void writeUInt32(uint32_t n) = 0;

    /// Reads a standard string.
    virtual std::string readString() = 0;

    /// Writes a standard string.
    virtual void writeString(const std::string & str) = 0;

    /// Reads a block of data.
    virtual void readData(void * data, size_t size) = 0;

    /// Writes a block of data.
    virtual void writeData(const void * data, size_t size) = 0;
};
