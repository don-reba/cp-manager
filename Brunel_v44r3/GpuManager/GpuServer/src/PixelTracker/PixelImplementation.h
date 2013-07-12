#ifndef TRACK
#define TRACK 1

#include <vector>
#include "GpuIpc/Api/Api.h"

void pixel_tracker_implementation(const PixelEvent & event, std::vector<GpuTrack>& result);

#endif
