#include "Tracker.h"

#include "FastTrackIpc/Api/TrackerId.h"

#include "FastTrackIpc/IProtocol.h"

Tracker::Tracker(IProtocol & protocol) :
		m_protocol (protocol) {
}

bool Tracker::isPrime(int32_t n) {
	m_protocol.writeInt32(TrackerId_isPrime);
	m_protocol.writeInt32(n);
	return m_protocol.readBool();
}
