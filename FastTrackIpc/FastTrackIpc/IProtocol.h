#pragma once

#include <stdint.h>
#include <string>
#include <vector>

// The communication protocol for findimg and findimgd.

/** @class IProtocol IProtocol.h FastTrackD/IProtocol.h
 * Service interface encapsulating the message protocol.
 */
class IProtocol {
  public:
    virtual ~IProtocol() {}

    /// Reads a boolean value.
    virtual bool readBool() = 0;

    /// Writes a boolean value.
    virtual void writeBool(bool b) = 0;

    /// Reads a boolean value.
    virtual char readChar() = 0;

    /// Writes a boolean value.
    virtual void writeChar(char c) = 0;

    /// Reads a double-precision floating point number.
    virtual double readDouble() = 0;

    /// Records a double-precision floating point number.
    virtual void writeDouble(double f) = 0;

    /// Reads a 16-bit integer.
    virtual int16_t readInt16() = 0;

    /// Writes a 16-bit integer.
    virtual void writeInt16(int16_t n) = 0;

    /// Reads a 32-bit integer.
    virtual int32_t readInt32() = 0;

    /// Writes a 32-bit integer.
    virtual void writeInt32(int32_t n) = 0;

    /// Reads a 64-bit integer.
    virtual int64_t readInt64() = 0;

    /// Writes a 32-bit integer.
    virtual void writeInt64(int64_t n) = 0;

    /// Reads a standard string.
    virtual std::string readString() = 0;

    /// Writes a standard string.
    virtual void writeString(const std::string & str) = 0;
    
    /// Writes a char* of arbitrary size
    virtual void write(char* dataPointer, int size) = 0;

    /// Reads a char* of arbitrary size
    virtual void read(char*& dataPointer, int size) = 0;
};

