//-----------------------------------------------------------------------------
// PrPixelSerialization
//
// 2014-03-10 : Alexey Badalov, Daniel Campora
//-----------------------------------------------------------------------------

#include "PrPixelSerialization.h"

#include <stdexcept>

using namespace std;

//------------------
// utility functions
//------------------

template<typename T>
const uint8_t * copy(
    const uint8_t  * buffer,
    std::vector<T> & collection,
    size_t           count) {
  std::copy(
    (const T *)buffer,
    (const T *)buffer + count,
    std::back_inserter(collection));
  return buffer + sizeof(T) * count;
}

template<typename T>
uint8_t * copy(const std::vector<T> & collection, uint8_t * buffer) {
  return (uint8_t *)std::copy(collection.begin(), collection.end(), (T*)buffer);
}

//------------------------------------
// PrPixelSerialization implementation
//------------------------------------

void PrPixelSerialization::cleanEvent(){
  m_lastAddedSensor = -1;

  m_event.noSensors = 0;
  m_event.noHits = 0;
  m_event.sensorZs.clear();
  m_event.sensorHitStarts.clear();
  m_event.sensorHitsNums.clear();
  m_event.hitIDs.clear();
  m_event.hitXs.clear();
  m_event.hitYs.clear();
  m_event.hitZs.clear();

  m_indexedHits.clear();
}

void PrPixelSerialization::addHit(PrPixelHit* hit, int sensorNum, int hitID, float hitX, float hitY, int hitZ){
  // std::cout << "addHit - sensorNum " << sensorNum << ", hitID " << hitID
  //   << ", hitX " << hitX << ", hitY " << hitY << ", hitZ " << hitZ << std::endl;
  if(sensorNum != m_lastAddedSensor){
    // Add a new sensor
    m_lastAddedSensor = sensorNum;
    m_event.sensorHitStarts.push_back(m_event.noHits);
    m_event.sensorHitsNums.push_back(1);
    m_event.sensorZs.push_back(hitZ);
    m_event.noSensors++;
  }
  else {
    m_event.sensorHitsNums[sensorNum]++;
  }

  m_event.hitIDs.push_back(hitID);
  m_event.hitXs.push_back(hitX);
  m_event.hitYs.push_back(hitY);
  m_event.hitZs.push_back(hitZ);
  m_event.noHits++;

  m_indexedHits[hitID] = hit;
}

void PrPixelSerialization::serializeEvent(Data & buffer) {
  // compute total size and allocate memory
  const size_t noSensorsSize       = sizeof(int);
  const size_t noHitsSize          = sizeof(int);
  const size_t sensorZsSize        = m_event.sensorZs.size() * sizeof(int);
  const size_t sensorHitStartsSize = m_event.sensorHitStarts.size() * sizeof(int);
  const size_t sensorHitsNumsSize  = m_event.sensorHitsNums.size() * sizeof(int);
  const size_t hitIDsSize          = m_event.hitIDs.size() * sizeof(int);
  const size_t hitXsSize           = m_event.hitXs.size() * sizeof(float);
  const size_t hitYsSize           = m_event.hitYs.size() * sizeof(float);
  const size_t hitZsSize           = m_event.hitZs.size() * sizeof(float);
  buffer.resize(noSensorsSize + noHitsSize + sensorZsSize + sensorHitStartsSize
      + sensorHitsNumsSize + hitIDsSize + hitXsSize + hitYsSize + hitZsSize);

  uint8_t * dst = (uint8_t *)&buffer[0];
  // serialize POD members
  *(int32_t *)dst = m_event.noSensors; dst += noSensorsSize;
  *(int32_t *)dst = m_event.noHits;    dst += noHitsSize;
  
  // serialize container contents
  dst = copy(m_event.sensorZs,        dst);
  dst = copy(m_event.sensorHitStarts, dst);
  dst = copy(m_event.sensorHitsNums,  dst);
  dst = copy(m_event.hitIDs,          dst);
  dst = copy(m_event.hitXs,           dst);
  dst = copy(m_event.hitYs,           dst);
  dst = copy(m_event.hitZs,           dst);

  std::cout << "noSensors " << m_event.noSensors << ", noHits " << m_event.noHits
    << ", sensorZsSize " << sensorZsSize << ", sensorHitStartsSize " << sensorHitStartsSize
    << ", sensorHitsNumsSize " << sensorHitsNumsSize << ", hitIDsSize " << hitIDsSize
    << ", hitXsSize " << hitXsSize << ", hitYsSize " << hitYsSize << ", hitZsSize " << hitZsSize
    << std::endl;

  assert(dst == &buffer[0] + buffer.size());
}

void PrPixelSerialization::deserializeTracks(
    const Data    & trackData,
    PrPixelTracks & tracks) {
  if (trackData.empty())
    throw runtime_error("empty track data");
  if (trackData.size() % sizeof(GpuTrack) != 0)
    throw runtime_error("invalid track data size");

  const GpuTrack * gpuTracks = reinterpret_cast<const GpuTrack*>(trackData.data());

  const size_t count = trackData.size() / sizeof(GpuTrack);

  assert(track.hitsNum < MAX_TRACK_SIZE);
  
  for (size_t i = 0; i != count; ++i) {
    int hitsNum = gpuTracks[i].hitsNum;
    if (hitsNum < 0 || hitsNum > MAX_TRACK_SIZE)
      throw runtime_error("invalid track hitsNum");
    tracks.push_back(PrPixelTrack(gpuTracks[i], m_indexedHits, m_event.hitIDs));
  }
}
