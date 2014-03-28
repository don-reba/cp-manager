
//-----------------------------------------------------------------------------
// GPUPixelSerializer
//
// 2014-03-10 : Daniel Campora
//-----------------------------------------------------------------------------

#include "PrPixelSerializer.h"

#include <algorithm>
#include <iterator>

void PrPixelSerializer::cleanEvent(){
    lastAddedSensor = -1;

    event.noSensors = 0;
    event.noHits = 0;
    event.sensorZs.clear();
    event.sensorHitStarts.clear();
    event.sensorHitsNums.clear();
    event.hitIDs.clear();
    event.hitXs.clear();
    event.hitYs.clear();
    event.hitZs.clear();
}

void PrPixelSerializer::addHit(int sensorNum, int hitID, float hitX, float hitY, int hitZ){
    if(sensorNum != lastAddedSensor){
        // Add a new sensor
        lastAddedSensor = sensorNum;
        event.sensorHitStarts.push_back(event.noHits);
        event.sensorHitsNums.push_back(1);
        event.sensorZs.push_back(hitZ);
    }
    else
        event.sensorHitsNums[sensorNum]++;

    event.hitIDs.push_back(hitID);
    event.hitXs.push_back(hitX);
    event.hitYs.push_back(hitY);
    event.hitZs.push_back(hitZ);
    event.noHits++;
}

void PrPixelSerializer::serialize(){
    // Reserve a chunk of memory, and copy to it
    int totalSize = sizeof(int)
        + sizeof(int)
        + event.sensorZs.size() * sizeof(int)
        + event.sensorHitStarts.size() * sizeof(int)
        + event.sensorHitsNums.size() * sizeof(int)
        + event.hitIDs.size() * sizeof(int)
        + event.hitXs.size() * sizeof(float)
        + event.hitYs.size() * sizeof(float)
        + event.hitZs.size() * sizeof(int);
    s.reserve(totalSize);

    // Add stuff to it
    std::copy_n((const uint8_t *)&event.noSensors,       sizeof(int),                                std::back_inserter(s));
    std::copy_n((const uint8_t *)&event.noHits,          sizeof(int),                                std::back_inserter(s));
    std::copy_n((const uint8_t *)&event.sensorZs,        event.sensorZs.size() * sizeof(int),        std::back_inserter(s));
    std::copy_n((const uint8_t *)&event.sensorHitStarts, event.sensorHitStarts.size() * sizeof(int), std::back_inserter(s));
    std::copy_n((const uint8_t *)&event.hitIDs,          event.hitIDs.size() * sizeof(int),          std::back_inserter(s));
    std::copy_n((const uint8_t *)&event.hitXs,           event.hitXs.size() * sizeof(float),         std::back_inserter(s));
    std::copy_n((const uint8_t *)&event.hitYs,           event.hitYs.size() * sizeof(float),         std::back_inserter(s));
    std::copy_n((const uint8_t *)&event.hitZs,           event.hitZs.size() * sizeof(int),           std::back_inserter(s));
}
