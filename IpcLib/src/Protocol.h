#pragma once

#include "IProtocol.h"

class ITransport;

class Protocol : public IProtocol {
	public:
    Protocol(ITransport & transport);

    virtual ~Protocol();

		/// Reads a 32-bit floating point number.
		virtual float readFloat();

		/// Records a 32-bit floating point number.
		virtual void writeFloat(float f);

		/// Reads a 32-bit integer.
		virtual int readInt();

		/// Writes a 32-bit integer.
		virtual void writeInt(int n);

		/// Reads a standard string.
		virtual std::string readString();

		/// Writes a standard string.
		virtual void writeString(const std::string & str);

  private:
    ITransport & m_transport;
};
