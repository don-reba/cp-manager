
#ifndef GPUPIXELSEARCHBYTRIPLET
#define GPUPIXELSEARCHBYTRIPLET 1

#include "FileStdLogger.h"
#include "Tools.cuh"
#include "KernelInvoker.cuh"

int independent_execute(const std::vector<std::vector<unsigned char> >& input, std::vector<std::vector<unsigned char> >& output);

void independent_post_execute(const std::vector<std::vector<unsigned char> >& output);

int gpuPixelSearchByTriplet(const std::vector<const std::vector<unsigned char>* >& input, std::vector<std::vector<unsigned char> >& output);

/**
 * Common entrypoint for Gaudi and non-Gaudi
 * @param input  
 * @param output 
 * @param logger 
 */
int gpuPixelSearchByTripletInvocation( const std::vector<std::vector<unsigned char> >& input,
    std::vector<std::vector<unsigned char> >& output, std::ostream& logger);

#endif
