#ifndef KERNEL_INVOKER
#define KERNEL_INVOKER 1

#include "Tools.cuh"

#include "Definitions.h"

#include <fstream>
#include <sstream>
#include <vector>
#include <stdint.h>

void getMaxNumberOfHits(char*& input, int& maxHits);
void printTrack(Track* tracks, int i, const Event & event, std::ostream& logger);
void printOutSensorHits(int sensorNumber, int* prevs, int* nexts, const Event & event, std::ostream& logger);
void printOutAllSensorHits(int* prevs, int* nexts, std::ostream& logger);
void printInfo(const Event & event, std::ostream& logger);

cudaError_t invokeParallelSearch(
    dim3                         numThreads,
    const std::vector<uint8_t> & input,
    std::vector<uint8_t>       & solution,
    std::ostream               & logger);

#endif
