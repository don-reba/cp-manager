#include "PatPixel.h"

#include "PatPixelSerialization/Serialization.h"
#include "PixelTracker/PixelImplementation.h"

#include <algorithm>
#include <iostream>

using namespace std;

namespace {
	void printBatchInfo(const Batch & batch) {
		cout << "'searchByPair' received";
		for (size_t i = 0, size = batch.size(); i != size; ++i)
			cout << ' ' << batch[i]->size();
		cout << " bytes" << endl;
	}
}

void Handlers::searchByPair(
    const Batch & batch,
    Alloc         allocResult,
    AllocParam    allocResultParam) {
	printBatchInfo(batch);

	// the current PatPixel implementation processes one event at a time
	for (size_t i = 0, size = batch.size(); i != size; ++i) {
		const Data & data = *batch[i];

		PixelEvent event;
		deserializePixelEvent(data, event);

		std::vector<GpuTrack> tracks;
		pixel_tracker_implementation(event, tracks);

		// TODO: move allocation out of serialization
		Data result;
		serializeGpuTracks(tracks, result);

		void * buffer = allocResult(i, result.size(), allocResultParam);
		copy(result.begin(), result.end(), (uint8_t*)buffer);
	}
}
