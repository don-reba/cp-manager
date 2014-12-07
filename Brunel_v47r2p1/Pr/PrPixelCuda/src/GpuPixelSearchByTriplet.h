#pragma once

#include <vector>
#include <cstdint>
#include <ostream>

typedef std::vector<uint8_t>     Data;
typedef std::vector<const Data*> Batch;

int independentExecute(const Batch & input, std::vector<Data> & output);

void independentPostExecute(const std::vector<Data>& output);

int gpuPixelSearchByTriplet(const Batch & input, std::vector<Data> & output);

/**
 * Common entry point for Gaudi and non-Gaudi
 * @param input  
 * @param output 
 * @param logger 
 */
int gpuPixelSearchByTripletInvocation(
    const Batch       & eventBatch,
    std::vector<Data> & trackBatch,
    std::ostream      & logger);
