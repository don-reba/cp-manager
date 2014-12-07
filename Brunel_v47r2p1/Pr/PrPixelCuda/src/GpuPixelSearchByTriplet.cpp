#include "GpuPixelSearchByTriplet.h"

#include "FileStdLogger.h"

#include "Tools.cuh"
#include "KernelInvoker.cuh"

using namespace std;

int independentExecute(const Batch & input, vector<Data>& output) {
  return gpuPixelSearchByTripletInvocation(input, output, cout);
}

void independentPostExecute(const vector<Data> & output) {
  cout << "post_execute invoked" << endl;
  cout << "Size of output: " << output.size() << " B" << endl;

  // Do some printing
}

int gpuPixelSearchByTriplet(const Batch & input, vector<Data> & output) {
  FileStdLogger discardStream;
  VoidLogger logger(&discardStream);

  // Silent execution
  // return gpuPixelSearchByTripletInvocation(input, output, discardStream);
  // 
  // Debug (with cout)
  return gpuPixelSearchByTripletInvocation(input, output, cout);
}

/**
 * Common entrypoint for Gaudi and non-Gaudi
 * @param input  
 * @param trackBatch 
 * @param logger
 */
int gpuPixelSearchByTripletInvocation(
    const Batch  & eventBatch,
    vector<Data> & trackBatch,
    ostream      & logger) {
  logger << "Invoking gpuPixelSearchByTriplet with " << eventBatch.size() << " events" << endl;

  // define how many blocks / threads we need to deal with numberofevents

  // for each event, we will execute 48 blocks and 32 threads.
  // [ where do these magic numbers come from? ]
  // call a kernel for each event, let cuda engine decide when to issue the kernels.
  dim3 numBlocks(46), numThreads(32);

  // each execution will return a different output
  trackBatch.resize(eventBatch.size());

  // this should be done in streams (non-blocking)
  for (int i=0; i<eventBatch.size(); ++i)
    cudaCheck(invokeParallelSearch(numBlocks, numThreads, *eventBatch[i], trackBatch[i], logger));

  cudaCheck(cudaDeviceReset());

  return 0;
}
