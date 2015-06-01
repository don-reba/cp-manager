
#ifndef GPUPIXELSEARCHBYTRIPLET
#define GPUPIXELSEARCHBYTRIPLET 1

#include "FileStdLogger.h"
#include "Tools.cuh"
#include "KernelInvoker.cuh"
#include "Logger.h"

#include <stdint.h>

int independent_execute(
    const std::vector<Data> & input,
    std::vector<Data>       & output);

void independent_post_execute(const std::vector<Data> & output);

int gpuPixelSearchByTriplet(
    const std::vector<const Data* > & input,
    std::vector<Data>               & output);

/**
 * Common entrypoint for Gaudi and non-Gaudi
 * @param input
 * @param output
 */
int gpuPixelSearchByTripletInvocation(
    const std::vector<const Data*> & input,
    std::vector<Data>              & output);

#endif
