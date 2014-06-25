#include "PixelImplementation.h"
#include "PrPixelCudaHandler.h"
#include "PrPixelSerialization/Serialization.h"

#include <iostream>

using namespace std;

DECLARE_COMPONENT(PrPixelCudaHandler)

void PrPixelCudaHandler::operator() (
    const Batch & batch,
    Alloc         allocResult,
    AllocParam    allocResultParam) {
  // the current PrPixel implementation processes one event at a time
  for (size_t i = 0, size = batch.size(); i != size; ++i) {
    const Data & data = *batch[i];

    PixelEvent event;
    deserializePixelEvent(data, event);
    //printEvent(event);

    vector<GpuTrack> tracks;
    pixel_tracker_implementation(event, tracks);

    // TODO: move allocation out of serialization
    Data result;
    serializeGpuTracks(tracks, result);

    void * buffer = allocResult(i, result.size(), allocResultParam);
    copy(result.begin(), result.end(), (uint8_t*)buffer);
  }

}
