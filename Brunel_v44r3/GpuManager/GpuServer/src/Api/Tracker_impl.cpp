// Automatically generated file
#include "TrackerServer.h"
#include "GpuIpc/Api/Api.h"
#include "../PixelTracker/PixelImplementation.h"

// service functions for the user to implement
void TrackerServer::searchByPair(const PixelEvent & event, std::vector<GpuTrack> & tracks) {
  pixel_tracker_implementation(event, tracks);
}
