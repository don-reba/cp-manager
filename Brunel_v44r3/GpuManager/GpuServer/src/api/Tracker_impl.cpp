// Automatically generated file
#include "Tracker.h"
#include "GpuIpc/api/api.h"
#include "../PixelTracker/PixelImplementation.h"

// service functions for the user to implement
void Tracker::searchByPair(const PixelEvent & event, std::vector<GpuTrack> & tracks) const {
  pixel_tracker_implementation(event, tracks);
}
