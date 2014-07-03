#include "GpuPixelSearchByTriplet.h"

int independent_execute(const Batch & input, std::vector<Data>& output) {
  return gpuPixelSearchByTripletInvocation(input, output, std::cout);
}

void independent_post_execute(
    const std::vector<Data> & output) {
  std::cout << "post_execute invoked" << std::endl;
  std::cout << "Size of output: " << output.size() << " B" << std::endl;

  // Do some printing
}

int gpuPixelSearchByTriplet(const Batch & input, std::vector<Data> & output) {
  FileStdLogger discardStream;
  VoidLogger logger(&discardStream);

  // Silent execution
  return gpuPixelSearchByTripletInvocation(input, output, discardStream);
}

/**
 * Common entrypoint for Gaudi and non-Gaudi
 * @param input  
 * @param output 
 * @param logger
 */
int gpuPixelSearchByTripletInvocation(
    const Batch & input,
    std::vector<Data>& output,
    std::ostream& logger) {
  logger << "Invoking gpuPixelSearchByTriplet with " << input.size() << " events" << std::endl;

  // Define how many blocks / threads we need to deal with numberOfEvents

  // For each event, we will execute 48 blocks and 32 threads.
  // Call a kernel for each event, let CUDA engine decide when to issue the kernels.
  dim3 numBlocks(46), numThreads(32);

  // In principle, each execution will return a different output
  output.resize(input.size());

    // This should be done in streams (non-blocking)
  for (int i=0; i<input.size(); ++i)
    cudaCheck(invokeParallelSearch(numBlocks, numThreads, *input[i], output[i], logger));

  cudaCheck(cudaDeviceReset());

  // Deprecated:
  // Merge all solutions!
  // logger << "Merging solutions..." << std::endl;
  // mergeSolutions(solutions, output);

  return 0;
}
