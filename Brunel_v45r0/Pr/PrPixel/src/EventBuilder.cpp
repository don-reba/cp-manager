
//-----------------------------------------------------------------------------
// GPUPixelSerializer
//
// 2014-03-10 : Daniel Campora
//-----------------------------------------------------------------------------

#include "EventBuilder.h"

#include <algorithm>
#include <iterator>
#include <exception>
#include <iostream>

void EventBuilder::cleanEvent(){
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
}

void EventBuilder::addHit(int sensorNum, int hitID, float hitX, float hitY, int hitZ){
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
}
