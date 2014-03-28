
#pragma once

//-----------------------------------------------------------------------------
// PrPixelSerializer
//
// 2014-03-10 : Daniel Campora
//-----------------------------------------------------------------------------

#include "../PrPixel/PixelEvent.h" // TODO

#include <vector>

#include <stdint.h>

class PrPixelSerializer {
private:
	int lastAddedSensor;

public:
  std::vector<uint8_t> s;
  PixelEvent event;

  void cleanEvent();
  void addHit(int sensorNum, int hitID, float hitX, float hitY, int hitZ);
  void serialize();
  // void* allocTracks(size_t size, void* param);
};
