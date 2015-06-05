//-----------------------------------------------------------------------------
// PrPixelSerializer
//
// 2014-03-10 : Alexey Badalov, Daniel Campora
//-----------------------------------------------------------------------------

#pragma once

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
  void addHit(PrPixelHit* hit, int sensorNum, int hitID, float hitX, float hitY, float hitZ);
  void serializeEvent(Data & buffer);
  void deserializeTracks(const Data & trackData, PrPixelTracks & m_tracks);

private:
  void print(const PixelEvent & event) const;
  void print(const GpuTrack * tracks, size_t n) const;

private:
  PixelEvent                 m_event;
  int                        m_lastAddedSensor;
  std::map<int, PrPixelHit*> m_indexedHits;
};
