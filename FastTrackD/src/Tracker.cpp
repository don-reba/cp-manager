#include "Tracker.h"

#include "FastTrackIpc/Api/TrackerId.h"

#include <iostream>

//-------------------------------
// service functions to implement
//-------------------------------

bool Tracker::isPrime(int32_t n) const {
	// trial division algorithm
	if (n < 2)
		return false;
	for (int i = 2; i != n; ++i) {
		if (n % i == 0)
			return false;
	}
	return true;
}

//----------------------------------------
// automatically generated - do not modify
//----------------------------------------

bool Tracker::process(IProtocol & protocol) const {
	int id = protocol.readInt32();
	switch (id)
	{
		case TrackerId_isPrime:
			processIsPrime(protocol);
			break;
		default:
			std::cout << "Tracker::process: unknown id - '" << id << "'\n'";
	}
	return true;
}

void Tracker::processIsPrime(IProtocol & protocol) const {
	int32_t n = protocol.readInt32();
	protocol.writeBool(isPrime(n));
}
