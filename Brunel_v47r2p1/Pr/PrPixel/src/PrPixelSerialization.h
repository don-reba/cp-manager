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

#include "PrPixelHit.h"
#include "PrPixelTrack.h"

#include "PrPixelTypes.h"
#include "PatKernel/IPatDebugTool.h"

class PrPixelSerialization {
public:
  typedef std::vector<uint8_t> Data;

public:
  void cleanEvent();
  void addHit(PrPixelHit* hit, int sensorNum, int hitID, float hitX, float hitY, int hitZ);
  void serializeEvent(Data & buffer);
  void deserializeTracks(const Data & trackData, PrPixelTracks & m_tracks);

private:
  PixelEvent m_event;
  int m_lastAddedSensor;
  std::map<int, PrPixelHit*> m_indexedHits;
};

#endif
