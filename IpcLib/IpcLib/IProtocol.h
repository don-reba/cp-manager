#pragma once

#include <string>

// The communication protocol for findimg and findimgd.

/** @class IProtocol IProtocol.h FastTrackD/IProtocol.h
 * Service interface encapsulating the message protocol.
 */
class IProtocol {
	public:
    virtual ~IProtocol() {}

		/// Reads a 32-bit floating point number.
		virtual float readFloat() = 0;

		/// Records a 32-bit floating point number.
		virtual void writeFloat(float f) = 0;

		/// Reads a 32-bit integer.
		virtual int readInt() = 0;

		/// Writes a 32-bit integer.
		virtual void writeInt(int n) = 0;

		/// Reads a standard string.
		virtual std::string readString() = 0;

		/// Writes a standard string.
		virtual void writeString(const std::string & str) = 0;
};

