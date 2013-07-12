// Automatically generated file
#include "Api/TrackerServer.h"
#include "PixelTracker/PixelImplementation.h"

// service functions for the user to implement
void TrackerServer::searchByPair(const PixelEvent & event, std::vector<GpuTrack> & tracks) {
  pixel_tracker_implementation(event, tracks);
}
