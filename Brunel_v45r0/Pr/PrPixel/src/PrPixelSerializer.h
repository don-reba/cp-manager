
#pragma once

//-----------------------------------------------------------------------------
// PrPixelSerializer
//
// 2014-03-10 : Daniel Campora
//-----------------------------------------------------------------------------

#include "../PrPixel/PixelEvent.h" // TODO

#include <cstdint>
#include <vector>
#include <memory>

class PrPixelSerializer {
private:
	int lastAddedSensor;
    std::shared_ptr<std::vector<uint8_t>> s;

public:
    // TODO: Make std::shared_ptr
    PixelEvent event;

    PrPixelSerializer() : s(new std::vector<uint8_t>) {}

    void cleanEvent();
    void addHit(int sensorNum, int hitID, float hitX, float hitY, int hitZ);
    std::shared_ptr<std::vector<std::uint8_t>> serialize();
};
