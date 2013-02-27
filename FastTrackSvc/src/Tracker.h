#pragma once

#include <stdint.h>

class IProtocol;

class Tracker {
	public:

		Tracker(IProtocol & protocol);

		bool isPrime(int32_t n);

	private:

		IProtocol & m_protocol;
};
