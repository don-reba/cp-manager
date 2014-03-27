
#pragma once

//-----------------------------------------------------------------------------
// PrPixelSerializer
//
// 2014-03-10 : Daniel Campora
//-----------------------------------------------------------------------------

#include "Serializer.h"
#include "../PrPixel/PixelEvent.h" // TODO

class PrPixelSerializer {
private:
	int lastAddedSensor;

public:
    Serializer* s;
    PixelEvent event;

    PrPixelSerializer(){ s = new Serializer(); }
    ~PrPixelSerializer(){ delete s; }

    void cleanEvent();
    void addHit(int sensorNum, int hitID, float hitX, float hitY, int hitZ);
    void serialize();
    // void* allocTracks(size_t size, void* param);
};
