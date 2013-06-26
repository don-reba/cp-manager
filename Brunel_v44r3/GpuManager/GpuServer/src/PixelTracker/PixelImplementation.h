#ifndef TRACK
#define TRACK 1

#include <vector>
#include "GpuIpc/api/api.h"

void pixel_tracker_implementation(const PixelEvent & event, std::vector<GpuTrack>& result);

#endif
