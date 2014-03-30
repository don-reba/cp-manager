#include "PrPixel.h"

#include "PrPixelSerialization/Serialization.h"
#include "PrPixel/PixelImplementation.h"

#include <algorithm>
#include <iostream>

using namespace Handlers;
using namespace std;

namespace {
  void printBatchInfo(const Batch & batch) {
    cout << "'tripletSearchGPU' received";
    for (size_t i = 0, size = batch.size(); i != size; ++i)
      cout << ' ' << batch[i]->size();
    cout << " bytes" << endl;
  }
}

void Handlers::tripletSearchGPU(
    const Batch & batch,
    Alloc         allocResult,
    AllocParam    allocResultParam) {
  //printBatchInfo(batch);

  // the current PrPixel implementation processes one event at a time
  for (size_t i = 0, size = batch.size(); i != size; ++i) {
    const Data & data = *batch[i];

    cout << "hi there" << endl;
    cout << "data:           " << data.size()                 << " B" << endl;

    PixelEvent event;
    deserializePixelEvent(data, event);

    cout << "noSensors:      " << event.noSensors             << endl;
    cout << "noHits:         " << event.noHits                << endl;
    cout << "sensorZs:       " << event.sensorZs.size()       << endl;
    cout << "ensorHitStarts: " << event.sensorHitStarts.size() << endl;
    cout << "ensorHitsNums:  " << event.sensorHitsNums.size()  << endl;
    cout << "itIDs:          " << event.hitIDs.size()          << endl;
    cout << "itXs:           " << event.hitXs.size()           << endl;
    cout << "itYs:           " << event.hitYs.size()           << endl;
    cout << "itZs:           " << event.hitZs.size()           << endl;

    std::vector<GpuTrack> tracks;
    pixel_tracker_implementation(event, tracks);

    // TODO: move allocation out of serialization
    Data result;
    serializeGpuTracks(tracks, result);

    void * buffer = allocResult(i, result.size(), allocResultParam);
    copy(result.begin(), result.end(), (uint8_t*)buffer);
  }
}
