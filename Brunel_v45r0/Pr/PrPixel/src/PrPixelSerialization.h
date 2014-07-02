
#ifndef PRPIXEL_SERIALIZATION
#define PRPIXEL_SERIALIZATION 1

//-----------------------------------------------------------------------------
// PrPixelSerializer
//
// 2014-03-10 : Alexey Badalov, Daniel Campora
//-----------------------------------------------------------------------------

#include <cstdint>
#include <vector>
#include <memory>

#include <algorithm>
#include <iterator>
#include <exception>
#include <iostream>

#include "PrPixelTrack.h"
#include "PrPixelTypes.h"
#include "PrPixelHit.h"
#include "PatKernel/IPatDebugTool.h"

//------------------
// utility functions
//------------------

template<typename T>
const uint8_t * copyHelper(const uint8_t * buffer, std::vector<T> & collection, size_t count) {
  std::copy(
    (const T *)buffer,
    (const T *)buffer + count,
    std::back_inserter(collection));
  return buffer + sizeof(T) * count;
}

template<typename T>
uint8_t * copyHelper(const std::vector<T> & collection, uint8_t * buffer) {
  return (uint8_t *)std::copy(collection.begin(), collection.end(), (T*)buffer);
}

class PrPixelSerialization {
public:
  void cleanEvent();
  void addHit(PrPixelHit* hit, int sensorNum, int hitID, float hitX, float hitY, int hitZ);
  void serializeEvent(std::vector<uint8_t> & buffer);
  void deserializeTracks(const std::vector<uint8_t> & trackCollection, PrPixelTracks & m_tracks);

private:
  PixelEvent m_event;
  int m_lastAddedSensor;
  std::map<int, PrPixelHit*> indexedHits;
};

#endif
