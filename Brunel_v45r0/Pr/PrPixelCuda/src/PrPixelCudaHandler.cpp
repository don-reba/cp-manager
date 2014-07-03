#include "GpuPixelSearchByTriplet.h"
#include "PrPixelCudaHandler.h"
// #include "PrPixelSerialization/Serialization.h"

#include <vector>

DECLARE_COMPONENT(PrPixelCudaHandler)

void PrPixelCudaHandler::operator() (
    const Batch & batch,
    Alloc         allocResult,
    AllocParam    allocResultParam) {

    // gpuPixelSearchByTriplet handles several events in parallel
    std::vector<Data> trackCollection;
    gpuPixelSearchByTriplet(batch, trackCollection);

    // TODO: Why do we need to copy these additionally from trackCollection to a[nother] buffer?
    // The results are already allocated in the server side, on trackCollection.
    for (int i=0, size=trackCollection.size(); i<size; ++i){
        uint8_t * buffer = (uint8_t*) allocResult(i, trackCollection[i].size(), &trackCollection);
        std::copy(trackCollection[i].begin(), trackCollection[i].end(), buffer);
    }
}
