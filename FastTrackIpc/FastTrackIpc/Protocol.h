#pragma once

#include "IProtocol.h"

class ITransport;

class Protocol : public IProtocol {
  public: // public interface

    Protocol(ITransport & transport);

    virtual ~Protocol();

  public: // IProtocol implementation

    /// Reads a boolean value.
    virtual bool readBool();

    /// Writes a boolean value.
    virtual void writeBool(bool b);

    /// Reads a double-precision floating point number.
    virtual double readDouble();

    /// Records a double-precision floating point number.
    virtual void writeDouble(double f);

    /// Reads an 8-bit integer.
    virtual int8_t readInt8();

    /// Writes an 8-bit integer.
    virtual void writeInt8(int8_t n);

    /// Reads a 16-bit integer.
    virtual int16_t readInt16();

    /// Writes a 16-bit integer.
    virtual void writeInt16(int16_t n);

    /// Reads a 32-bit integer.
    virtual int32_t readInt32();

    /// Writes a 32-bit integer.
    virtual void writeInt32(int32_t n);

    /// Reads a 64-bit integer.
    virtual int64_t readInt64();

    /// Writes a 32-bit integer.
    virtual void writeInt64(int64_t n);

    /// Reads a standard string.
    virtual std::string readString();

    /// Writes a standard string.
    virtual void writeString(const std::string & str);


  private:
    ITransport & m_transport;
};

