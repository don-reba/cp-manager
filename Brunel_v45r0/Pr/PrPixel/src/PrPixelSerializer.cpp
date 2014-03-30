
//-----------------------------------------------------------------------------
// GPUPixelSerializer
//
// 2014-03-10 : Daniel Campora
//-----------------------------------------------------------------------------

#include "PrPixelSerializer.h"

#include <algorithm>
#include <iterator>
#include <exception>
#include <iostream>

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
    // std::cout << "addHit: sensorNum " << sensorNum << ", hitID " << hitID << ", hitX " << hitX
    //     << ", hitY " << hitY << ", hitZ " << hitZ << std::endl;

    if(sensorNum != lastAddedSensor){
        // Add a new sensor
        lastAddedSensor = sensorNum;
        event.sensorHitStarts.push_back(event.noHits);
        event.sensorHitsNums.push_back(1);
        event.sensorZs.push_back(hitZ);
        event.noSensors++;
    }
    else {
        event.sensorHitsNums[sensorNum]++;
    }

    event.hitIDs.push_back(hitID);
    event.hitXs.push_back(hitX);
    event.hitYs.push_back(hitY);
    event.hitZs.push_back(hitZ);
    event.noHits++;
}

std::shared_ptr<std::vector<uint8_t>> PrPixelSerializer::serialize(){
    // std::cout << "serialize: noSensors " << event.noSensors << ", noHits " << event.noHits << ", sensorZs " << event.sensorZs[0]
    //     << ", sensorHitStarts " << event.sensorHitStarts[0] << ", hitIDs " << event.hitIDs[0] << ", hitXs " << event.hitXs[0]
    //     << ", hitYs " << event.hitYs[0] << ", hitZs " << event.hitZs[0] << std::endl;

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
    s->reserve(totalSize);

    std::copy_n((const uint8_t *)&event.noSensors,       sizeof(int),                                std::back_inserter(*s));
    std::copy_n((const uint8_t *)&event.noHits,          sizeof(int),                                std::back_inserter(*s));
    std::copy_n((const uint8_t *)&(event.sensorZs[0]),        event.sensorZs.size() * sizeof(int),       std::back_inserter(*s));
    std::copy_n((const uint8_t *)&(event.sensorHitStarts[0]), event.sensorHitStarts.size() * sizeof(int), std::back_inserter(*s));
    std::copy_n((const uint8_t *)&(event.hitIDs[0]),          event.hitIDs.size() * sizeof(int),          std::back_inserter(*s));
    std::copy_n((const uint8_t *)&(event.hitXs[0]),           event.hitXs.size() * sizeof(float),         std::back_inserter(*s));
    std::copy_n((const uint8_t *)&(event.hitYs[0]),           event.hitYs.size() * sizeof(float),         std::back_inserter(*s));
    std::copy_n((const uint8_t *)&(event.hitZs[0]),           event.hitZs.size() * sizeof(int),           std::back_inserter(*s));

    // int* int_pointer = (int*) &((*s)[0]);
    // for (int i=0; i<s->size() / 4; ++i)
    //   std::cout << (int) int_pointer[i] << ", ";
    // std::cout << std::endl;

    return s;

    // try {
    //     std::cout << "Copy success! :)" << std::endl;
    // }
    // catch (std::exception& e) {
    //     std::cout << "Exception: " << e.what() << std::endl;
    // }
}
