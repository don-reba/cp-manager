#pragma once

#include "GpuTrack.h"
#include "PixelEvent.h"

#include <vector>
#include <stdint.h>

void serializeGpuTracks(const std::vector<GpuTrack> & tracks, std::vector<uint8_t> & buffer);

void deserializeGpuTracks(const std::vector<uint8_t> & buffer, std::vector<GpuTrack> & tracks);

void serializeEvent(const PixelEvent & event, std::vector<uint8_t> & buffer);

void deserializePixelEvent(const std::vector<uint8_t> & buffer, PixelEvent & event);
