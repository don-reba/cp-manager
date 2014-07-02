
#include "GpuPixelSearchByTriplet.h"

int independent_execute(const std::vector<const std::vector<unsigned char>* >& input, std::vector<std::vector<unsigned char> >& output){
    return gpuPixelSearchByTripletInvocation(input, output, std::cout);
}

void independent_post_execute(const std::vector<std::vector<unsigned char> >& output){
    std::cout << "post_execute invoked" << std::endl;
    std::cout << "Size of output: " << output.size() << " B" << std::endl;

    // Do some printing
}

int gpuPixelSearchByTriplet(const std::vector<const std::vector<unsigned char>* >& input, std::vector<std::vector<unsigned char> >& output){
    FileStdLogger discardStream;
    VoidLogger logger (&discardStream);

    // Silent execution
    return gpuPixelSearchByTripletInvocation(input, output, discardStream);
}

/**
 * Common entrypoint for Gaudi and non-Gaudi
 * @param input  
 * @param output 
 * @param logger
 */
int gpuPixelSearchByTripletInvocation(const std::vector<const std::vector<unsigned char>* >& input,
    std::vector<std::vector<unsigned char> >& output, std::ostream& logger){

    logger << "Invoking gpuPixelSearchByTriplet with " << input.size() << " events" << std::endl;

    // Define how many blocks / threads we need to deal with numberOfEvents
    
    // For each event, we will execute 48 blocks and 32 threads.
    // Call a kernel for each event, let CUDA engine decide when to issue the kernels.
    dim3 numBlocks(46), numThreads(32);

    // In principle, each execution will return a different output
    output.resize(input.size());
    
    for (int i=0; i<input.size(); ++i){
        // This should be done in streams (non-blocking)
        cudaCheck( invokeParallelSearch(numBlocks, numThreads, *(input[i]), output[i], logger) );
    }
    
    cudaCheck( cudaDeviceReset() );
   
    // Deprecated:
    // Merge all solutions!
    // logger << "Merging solutions..." << std::endl;
    // mergeSolutions(solutions, output);

    return 0;
}
