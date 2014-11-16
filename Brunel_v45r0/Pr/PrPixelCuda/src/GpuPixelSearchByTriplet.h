#ifndef GPUPIXELSEARCHBYTRIPLET
#define GPUPIXELSEARCHBYTRIPLET 1

#include "FileStdLogger.h"
#include "Tools.cuh"
#include "KernelInvoker.cuh"

#include <stdint.h>

typedef std::vector<uint8_t>     Data;
typedef std::vector<const Data*> Batch;

int independent_execute(const Batch & input, std::vector<Data> & output);

void independent_post_execute(const std::vector<Data>& output);

int gpuPixelSearchByTriplet(const Batch & input, std::vector<Data> & output);

/**
 * Common entrypoint for Gaudi and non-Gaudi
 * @param input
 * @param output
 * @param logger
 */
int gpuPixelSearchByTripletInvocation(
    const Batch       & input,
    std::vector<Data> & output,
    std::ostream      & logger);

#endif
