#include "PatPixel.h"

#include "PatPixelSerialization/Serialization.h"
#include "PixelTracker/PixelImplementation.h"

#include <algorithm>
#include <iostream>

using namespace std;

void Handlers::searchByPair(
    const Data & data,
    Alloc        allocResult,
    AllocParam   allocResultParam) {
  cout << "'searchByPair' received " << data.size() << " bytes" << endl;

  PixelEvent event;
  deserializePixelEvent(data, event);

  std::vector<GpuTrack> tracks;
  pixel_tracker_implementation(event, tracks);

  // TODO: move allocation out of serialization
  Data result;
  serializeGpuTracks(tracks, result);

  void * buffer = allocResult(result.size(), allocResultParam);
  copy(result.begin(), result.end(), (uint8_t*)buffer);
}
