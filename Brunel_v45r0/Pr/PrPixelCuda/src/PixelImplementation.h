#pragma once

#include "PrPixelSerialization/Serialization.h"

#include <vector>

void pixel_tracker_implementation(
    const PixelEvent      & event,
    std::vector<GpuTrack> & result);
