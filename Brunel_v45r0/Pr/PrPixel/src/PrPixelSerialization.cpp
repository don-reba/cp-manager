
//-----------------------------------------------------------------------------
// PrPixelSerialization
//
// 2014-03-10 : Alexey Badalov, Daniel Campora
//-----------------------------------------------------------------------------

#include "PrPixelSerialization.h"

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

  indexedHits.clear();
}

void PrPixelSerialization::addHit(PrPixelHit* hit, int sensorNum, int hitID, float hitX, float hitY, int hitZ){
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

  indexedHits[hitID] = hit;
}

void PrPixelSerialization::serializeEvent(std::vector<uint8_t> & buffer) {
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
  dst = copyHelper(m_event.sensorZs,        dst);
  dst = copyHelper(m_event.sensorHitStarts, dst);
  dst = copyHelper(m_event.sensorHitsNums,  dst);
  dst = copyHelper(m_event.hitIDs,          dst);
  dst = copyHelper(m_event.hitXs,           dst);
  dst = copyHelper(m_event.hitYs,           dst);
  dst = copyHelper(m_event.hitZs,           dst);

  assert(dst == &buffer[0] + buffer.size());
}

void PrPixelSerialization::deserializeTracks(const std::vector<uint8_t> & trackCollection, PrPixelTracks & m_tracks){
  std::vector<PixelTrack> * trackCollectionPointer = (std::vector<PixelTrack>*) &(trackCollection);

  if (trackCollectionPointer->empty()){
    // TODO: error()
    std::cerr << "Returned track collection is empty!" << std::endl;
  }
  else {
    for (size_t i=0; i<trackCollectionPointer->size(); ++i){
      m_tracks.push_back( PrPixelTrack( &((*trackCollectionPointer)[i]), indexedHits, m_event.hitIDs ) );
    }
  }
}
