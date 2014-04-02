
#pragma once

//-----------------------------------------------------------------------------
// EventBuilder
//
// 2014-03-10 : Daniel Campora
//-----------------------------------------------------------------------------

#include "PrPixelSerialization/PixelEvent.h" // TODO

#include <cstdint>
#include <vector>
#include <memory>

class EventBuilder {
public:

	void cleanEvent();
	void addHit(int sensorNum, int hitID, float hitX, float hitY, int hitZ);

	const PixelEvent & getEvent() { return m_event; }

private:
	PixelEvent m_event;
	int m_lastAddedSensor;
};
