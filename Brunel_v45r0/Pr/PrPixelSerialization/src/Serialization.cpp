#include "Serialization.h"

#include <algorithm>
#include <cassert>
#include <iterator>

using namespace std;

//------------------
// utility functions
//------------------

template<typename T>
const uint8_t * copy(const uint8_t * buffer, vector<T> & collection, size_t count) {
  std::copy(
    (const T *)buffer,
    (const T *)buffer + count,
    back_inserter(collection));
  return buffer + sizeof(T) * count;
}

template<typename T>
uint8_t * copy(const vector<T> & collection, uint8_t * buffer) {
  return (uint8_t *)std::copy(collection.begin(), collection.end(), (T*)buffer);
}

//----------------
// serialiazastion
//----------------

void serializeGpuTracks(const vector<GpuTrack> & tracks, vector<uint8_t> & buffer) {
  // compute total size and allocate memory
  size_t totalSize = 4; // 4 bytes for the track count
  for (vector<GpuTrack>::const_iterator track = tracks.begin(), end = tracks.end(); track != end; ++track) {
    const size_t x0Size  = 4; // 1
    const size_t txSize  = 4; // 2
    const size_t y0Size  = 4; // 3
    const size_t tySize  = 4; // 4
    const size_t s0Size  = 4; // 5
    const size_t sxSize  = 4; // 6
    const size_t szSize  = 4; // 7
    const size_t sxzSize = 4; // 8
    const size_t sz2Size = 4; // 9
    const size_t u0Size  = 4; // 10
    const size_t uySize  = 4; // 11
    const size_t uzSize  = 4; // 12
    const size_t uyzSize = 4; // 13
    const size_t uz2Size = 4; // 14
    const size_t trackHitsNumSize = 4;
    const size_t hitsSize = 4 + 4 * track->hits.size();

    totalSize += x0Size + txSize + y0Size + tySize + s0Size + sxSize + szSize +
      sxzSize + sz2Size + u0Size + uySize + uzSize + uyzSize + uz2Size +
      trackHitsNumSize + hitsSize;
  }
  buffer.resize(totalSize);

  // serialize track count
  uint8_t * dst = (uint8_t *)&buffer[0];
  *(uint32_t *)dst = tracks.size(); dst += 4;
  // serialize tracks
  for (vector<GpuTrack>::const_iterator track = tracks.begin(), end = tracks.end(); track != end; ++track) {
    *(float *)dst = track->x0;  dst += 4; // 1
    *(float *)dst = track->tx;  dst += 4; // 2
    *(float *)dst = track->y0;  dst += 4; // 3
    *(float *)dst = track->ty;  dst += 4; // 4
    *(float *)dst = track->s0;  dst += 4; // 5
    *(float *)dst = track->sx;  dst += 4; // 6
    *(float *)dst = track->sz;  dst += 4; // 7
    *(float *)dst = track->sxz; dst += 4; // 8
    *(float *)dst = track->sz2; dst += 4; // 9
    *(float *)dst = track->u0;  dst += 4; // 10
    *(float *)dst = track->uy;  dst += 4; // 11
    *(float *)dst = track->uz;  dst += 4; // 12
    *(float *)dst = track->uyz; dst += 4; // 13
    *(float *)dst = track->uz2; dst += 4; // 14
    *(int32_t *)dst = track->trackHitsNum; dst += 4;

    *(uint32_t *)dst = track->hits.size(); dst += 4;
    dst = copy(track->hits, dst);
  }

  assert(dst == &buffer[0] + buffer.size());
}

void deserializeGpuTracks(const vector<uint8_t> & buffer, vector<GpuTrack> & tracks) {
  const uint8_t * src = &buffer[0];

  tracks.resize(*(const uint32_t *)src); src += 4;
  for (vector<GpuTrack>::iterator track = tracks.begin(); track != tracks.end(); ++track) {
    // deserialize pod members
    track->x0  = *(const float *)src; src += 4;
    track->tx  = *(const float *)src; src += 4;
    track->y0  = *(const float *)src; src += 4;
    track->ty  = *(const float *)src; src += 4;
    track->s0  = *(const float *)src; src += 4;
    track->sx  = *(const float *)src; src += 4;
    track->sz  = *(const float *)src; src += 4;
    track->sxz = *(const float *)src; src += 4;
    track->sz2 = *(const float *)src; src += 4;
    track->u0  = *(const float *)src; src += 4;
    track->uy  = *(const float *)src; src += 4;
    track->uz  = *(const float *)src; src += 4;
    track->uyz = *(const float *)src; src += 4;
    track->uz2 = *(const float *)src; src += 4;
    track->trackHitsNum = *(const int32_t *)src; src += 4;
    // deserialize hit collection
    const uint32_t hitsCount = *(const uint32_t *)src; src += 4;
    track->hits.reserve(hitsCount);
    src = copy(src, track->hits, hitsCount);
  }

  assert(src == &buffer[0] + buffer.size());
}

void serializeEvent(const PixelEvent & event, vector<uint8_t> & buffer) {
  // compute total size and allocate memory
  const size_t noSensorsSize       = 4;
  const size_t noHitsSize          = 4;
  const size_t sensorZsSize        = 4 + 4 * event.sensorZs.size();        // 1
  const size_t sensorHitStartsSize = 4 + 4 * event.sensorHitStarts.size(); // 2
  const size_t sensorHitsNumsSize  = 4 + 4 * event.sensorHitsNums.size();  // 3
  const size_t hitIDsSize          = 4 + 4 * event.hitIDs.size();          // 4
  const size_t hitXsSize           = 4 + 4 * event.hitXs.size();           // 5
  const size_t hitYsSize           = 4 + 4 * event.hitYs.size();           // 6
  const size_t hitZsSize           = 4 + 4 * event.hitZs.size();           // 7
  buffer.resize(noSensorsSize + noHitsSize + sensorZsSize + sensorHitStartsSize
      + sensorHitsNumsSize + hitIDsSize + hitXsSize + hitYsSize + hitZsSize);

  uint8_t * dst = (uint8_t *)&buffer[0];
  // serialize POD members
  *(int32_t *)dst = event.noSensors; dst += 4;
  *(int32_t *)dst = event.noHits;    dst += 4;
  // serialize container sizes
  *(uint32_t *)dst = event.sensorZs.size();        dst += 4; // 1
  *(uint32_t *)dst = event.sensorHitStarts.size(); dst += 4; // 2
  *(uint32_t *)dst = event.sensorHitsNums.size();  dst += 4; // 3
  *(uint32_t *)dst = event.hitIDs.size();          dst += 4; // 4
  *(uint32_t *)dst = event.hitXs.size();           dst += 4; // 5
  *(uint32_t *)dst = event.hitYs.size();           dst += 4; // 6
  *(uint32_t *)dst = event.hitZs.size();           dst += 4; // 7
  // serialize container contents
  dst = copy(event.sensorZs,        dst); // 1
  dst = copy(event.sensorHitStarts, dst); // 2
  dst = copy(event.sensorHitsNums,  dst); // 3
  dst = copy(event.hitIDs,          dst); // 4
  dst = copy(event.hitXs,           dst); // 5
  dst = copy(event.hitYs,           dst); // 6
  dst = copy(event.hitZs,           dst); // 7

  assert(dst == &buffer[0] + buffer.size());
}

void deserializePixelEvent(const vector<uint8_t> & buffer, PixelEvent & event) {
  const uint8_t * src = &buffer[0];

  // deserialize POD members
  event.noSensors = *(const int32_t *)src; src += 4;
  event.noHits    = *(const int32_t *)src; src += 4;
  // deserialize container sizes
  const size_t sensorZsCount        = *(const uint32_t *)src; src += 4; // 1
  const size_t sensorHitStartsCount = *(const uint32_t *)src; src += 4; // 2
  const size_t sensorHitsNumsCount  = *(const uint32_t *)src; src += 4; // 3
  const size_t hitIDsCount          = *(const uint32_t *)src; src += 4; // 4
  const size_t hitXsCount           = *(const uint32_t *)src; src += 4; // 5
  const size_t hitYsCount           = *(const uint32_t *)src; src += 4; // 6
  const size_t hitZsCount           = *(const uint32_t *)src; src += 4; // 7
  // deserialize container contents
  src = copy(src, event.sensorZs,        sensorZsCount);        // 1
  src = copy(src, event.sensorHitStarts, sensorHitStartsCount); // 2
  src = copy(src, event.sensorHitsNums,  sensorHitsNumsCount);  // 3
  src = copy(src, event.hitIDs,          hitIDsCount);          // 4
  src = copy(src, event.hitXs,           hitXsCount);           // 5
  src = copy(src, event.hitYs,           hitYsCount);           // 6
  src = copy(src, event.hitZs,           hitZsCount);           // 7

  assert(src == &buffer[0] + buffer.size());
}
