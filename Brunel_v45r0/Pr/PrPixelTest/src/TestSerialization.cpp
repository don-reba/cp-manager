#include "PrPixelSerialization/Serialization.h"

#include <boost/test/unit_test.hpp>

#include <algorithm>
#include <ostream>
#include <stdint.h>

using namespace std;

//-------------------------------------------
// output operators for Boost unit test tools
//-------------------------------------------

namespace std {
  ostream & operator << (ostream & s, const PixelEvent & e) {
    return s << "PixelEvent{" << (long)&e << "}";
  }
  ostream & operator << (ostream & s, const GpuTrack & t) {
    return s << "GpuTrack{" << (long)&t << "}";
  }
}

//---------------------
// comparison operators
//---------------------

bool operator == (const PixelEvent & e1, const PixelEvent & e2) {
  if (e1.noSensors       != e2.noSensors)       return false;
  if (e1.noHits          != e2.noHits)          return false;
  if (e1.sensorZs        != e2.sensorZs)        return false;
  if (e1.sensorHitStarts != e2.sensorHitStarts) return false;
  if (e1.sensorHitsNums  != e2.sensorHitsNums)  return false;
  if (e1.hitIDs          != e2.hitIDs)          return false;
  if (e1.hitXs           != e2.hitXs)           return false;
  if (e1.hitYs           != e2.hitYs)           return false;
  if (e1.hitZs           != e2.hitZs)           return false;
  return true;
}

bool operator == (const GpuTrack & t1, const GpuTrack & t2) {
  if (t1.x0           != t2.x0)           return false;
  if (t1.tx           != t2.tx)           return false;
  if (t1.y0           != t2.y0)           return false;
  if (t1.ty           != t2.ty)           return false;
  if (t1.s0           != t2.s0)           return false;
  if (t1.sx           != t2.sx)           return false;
  if (t1.sz           != t2.sz)           return false;
  if (t1.sxz          != t2.sxz)          return false;
  if (t1.sz2          != t2.sz2)          return false;
  if (t1.u0           != t2.u0)           return false;
  if (t1.uy           != t2.uy)           return false;
  if (t1.uz           != t2.uz)           return false;
  if (t1.uyz          != t2.uyz)          return false;
  if (t1.uz2          != t2.uz2)          return false;
  if (t1.trackHitsNum != t2.trackHitsNum) return false;
  if (t1.hits         != t2.hits)         return false;
  return true;
}

bool operator != (const GpuTrack & t1, const GpuTrack & t2) {
  return !(t1 == t2);
}

//-----------------------
// initialization helpers
//-----------------------

// fill the vector with a few arbitrary numbers
template<typename T>
void initVector(vector<T> & v) {
  static int primes[] = { 2, 3, 5, 7, 11, 13, 17, 19, 23, 29 };
  v.resize(10);
  for (size_t i = 0, size = v.size(); i != size; ++i)
    v[i] = static_cast<T>(primes[i]);
}

// fill the PixelEvent object with arbitrary values
void initPixelEvent(PixelEvent & e) {
  e.noSensors = 2;
  e.noHits    = 3;
  initVector(e.sensorZs);
  initVector(e.sensorHitStarts);
  initVector(e.sensorHitsNums);
  initVector(e.hitIDs);
  initVector(e.hitXs);
  initVector(e.hitYs);
  initVector(e.hitZs);
}

// fill the GpuTrack collection with arbitrary values
void initGpuTracks(vector<GpuTrack> & tracks) {
  int n = 1;

  tracks.resize(10);
  for (vector<GpuTrack>::iterator i = tracks.begin(), end = tracks.end(); i != end; ++i) {
    i->x0  = n++;
    i->tx  = n++;
    i->y0  = n++;
    i->ty  = n++;
    i->s0  = n++;
    i->sx  = n++;
    i->sz  = n++;
    i->sxz = n++;
    i->sz2 = n++;
    i->u0  = n++;
    i->uy  = n++;
    i->uz  = n++;
    i->uyz = n++;
    i->uz2 = n++;
    i->trackHitsNum = n++;

    i->hits.resize(10);
    for (vector<int32_t>::iterator j = i->hits.begin(), end = i->hits.end(); j != end; ++j)
      *j = n++;
  }
}

//-----------
// unit tests
//-----------

BOOST_AUTO_TEST_CASE(Test_PixelEvent) {
  PixelEvent event1;
  initPixelEvent(event1);
  
  vector<uint8_t> serializedEvent;
  serializeEvent(event1, serializedEvent);

  PixelEvent event2;
  deserializePixelEvent(serializedEvent, event2);

  BOOST_CHECK_EQUAL(event1, event2);
}

BOOST_AUTO_TEST_CASE(Test_GpuTrack) {
  vector<GpuTrack> tracks1;
  initGpuTracks(tracks1);

  vector<uint8_t> serializedTracks;
  serializeGpuTracks(tracks1, serializedTracks);

  vector<GpuTrack> tracks2;
  deserializeGpuTracks(serializedTracks, tracks2);

  BOOST_CHECK_EQUAL_COLLECTIONS(tracks1.begin(), tracks1.end(), tracks2.begin(), tracks2.end());
}
