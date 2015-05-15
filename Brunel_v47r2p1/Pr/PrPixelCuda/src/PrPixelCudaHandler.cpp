#include "GpuPixelSearchByTriplet.h"
#include "PrPixelCudaHandler.h"

#include <algorithm>
#include <vector>

using namespace std;

DECLARE_COMPONENT(PrPixelCudaHandler)

void PrPixelCudaHandler::operator() (
    const Batch & batch,
    Alloc         allocResult,
    AllocParam    allocResultParam) {
  // gpuPixelSearchByTriplet handles several events in parallel
  vector<Data> trackCollection(batch.size());
  gpuPixelSearchByTriplet(batch, trackCollection);

  for (int i = 0, size = batch.size(); i != size; ++i) {
    const vector<Track> & tracks = * reinterpret_cast<const vector<Track>*>(&trackCollection[i]);
    const size_t    n   = tracks.size() * sizeof(Track);
    const uint8_t * src = reinterpret_cast<const uint8_t*>(tracks.data());
    uint8_t *       dst = allocResult(i, n, allocResultParam);
    copy(src, src + n, dst);
  }
}
