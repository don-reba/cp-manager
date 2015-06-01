#include "GpuPixelSearchByTriplet.h"

#include <algorithm>

int independent_execute(
    const std::vector<Data> & input,
    std::vector<Data>       & output) {

  std::vector<const Data* > converted_input;
  converted_input.resize(input.size());

  for (int i = 0; i < input.size(); ++i)
    converted_input[i] = &input[i];

  std::cout << std::fixed << std::setprecision(2);
  logger::ll.verbosityLevel = 3;

  return gpuPixelSearchByTripletInvocation(converted_input, output);
}

void independent_post_execute(const std::vector<std::vector<uint8_t> > & output) {
    DEBUG << "post_execute invoked" << std::endl;
    DEBUG << "Size of output: " << output.size() << " B" << std::endl;
}

int gpuPixelSearchByTriplet(
    const std::vector<const Data*> & input,
    std::vector<Data>              & output) {

  // Silent execution
  std::cout << std::fixed << std::setprecision(2);
  logger::ll.verbosityLevel = 0;
  return gpuPixelSearchByTripletInvocation(input, output);
}

/**
 * Common entrypoint for Gaudi and non-Gaudi
 * @param input
 * @param output
 */
int gpuPixelSearchByTripletInvocation(
    const std::vector<const Data* > & input,
    std::vector<Data>               & output) {
  DEBUG << "Invoking gpuPixelSearchByTriplet with " << input.size() << " events" << std::endl;

  // Execute maximum n number of events every time
  const int maxEventsPerKernel = 1000;

  for (int i = 0, size = input.size(); i < size; i += maxEventsPerKernel) {
    const int eventsToProcess = std::min(size - i, maxEventsPerKernel);
    cudaCheck(invokeParallelSearch(i, eventsToProcess, input, output));
  }

  cudaCheck(cudaDeviceReset());

  return 0;
}
