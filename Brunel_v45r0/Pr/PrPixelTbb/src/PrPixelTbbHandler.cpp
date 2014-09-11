#include "PrPixelTbbHandler.h"
#include "PixelTbb.h"

#include <algorithm>
#include <iostream>
#include <stdexcept>
#include <vector>

using namespace std;

DECLARE_COMPONENT(PrPixelTbbHandler)

typedef PrPixelTbbHandler::Data Data;

template<typename T>
const uint8_t * Assign(vector<T> & v, const uint8_t * p, int count) {
  v.assign(reinterpret_cast<const T *>(p), reinterpret_cast<const T *>(p) + count);
  return p + count * sizeof(T);
}

void DeserializeEvent(const Data & data, PixelEvent & event) {
  const uint8_t * p = data.data();
  event.noSensors = *reinterpret_cast<const int32_t*>(p); p += sizeof(int32_t);
  event.noHits    = *reinterpret_cast<const int32_t*>(p); p += sizeof(int32_t);
  p = Assign(event.sensorZs,        p, event.noSensors);
  p = Assign(event.sensorHitStarts, p, event.noSensors);
  p = Assign(event.sensorHitsNums,  p, event.noSensors);
  p = Assign(event.hitIDs,          p, event.noHits);
  p = Assign(event.hitXs,           p, event.noHits);
  p = Assign(event.hitYs,           p, event.noHits);
  p = Assign(event.hitZs,           p, event.noHits);
  if (p != data.data() + data.size())
    throw runtime_error("event deserialization error");
}

SolutionTrack ConvertTrack(const GpuTrack & track) {
  if (track.trackHitsNum != static_cast<int>(track.hits.size()))
    throw runtime_error("invalid hits num");
  if (track.trackHitsNum >= MAX_TRACK_SIZE)
    throw runtime_error("too many tracks");

  SolutionTrack solution;

  solution.x0 = track.m_x0;
  solution.tx = track.m_tx;
  solution.y0 = track.m_y0;
  solution.ty = track.m_ty;

  solution.hitsNum = track.trackHitsNum;
  copy(track.hits.begin(), track.hits.end(), solution.hits);

  return solution; // hopefully, NRV works
}

void SerializeTracks(const vector<GpuTrack> & tracks, Data & data) {
  data.resize(sizeof(SolutionTrack) * tracks.size());
  SolutionTrack * solution = reinterpret_cast<SolutionTrack*>(data.data());
  transform(tracks.begin(), tracks.end(), solution, ConvertTrack);
}

void PrPixelTbbHandler::operator() (
    const Batch & batch,
    Alloc         allocResult,
    AllocParam    allocResultParam) {
  for (size_t i = 0, size = batch.size(); i != size; ++i) {
    PixelEvent event;
    DeserializeEvent(*batch[i], event);

    vector<GpuTrack> tracks;

    pixel_tracker_implementation(event, tracks);

    Data serializedTracks;
    SerializeTracks(tracks, serializedTracks);

    cout << tracks.size() << " tracks\n";

    uint8_t * buffer = allocResult(i, serializedTracks.size(), allocResultParam);
    copy(serializedTracks.begin(), serializedTracks.end(), buffer);
  }
}
