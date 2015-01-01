#include "Tools.cuh"

#include <stdexcept>

void parseEvent(uint8_t * input, size_t size, Event & event) {
  uint8_t * end = input + size;

  event.no_sensors       = (int32_t*)input; input += sizeof(int32_t);
  event.no_hits          = (int32_t*)input; input += sizeof(int32_t);
  event.sensor_Zs        = (int32_t*)input; input += sizeof(int32_t) * *event.no_sensors;
  event.sensor_hitStarts = (int32_t*)input; input += sizeof(int32_t) * *event.no_sensors;
  event.sensor_hitNums   = (int32_t*)input; input += sizeof(int32_t) * *event.no_sensors;
  event.hit_IDs          = (int32_t*)input; input += sizeof(int32_t) * *event.no_hits;
  event.hit_Xs           = (float*)  input; input += sizeof(float)   * *event.no_hits;
  event.hit_Ys           = (float*)  input; input += sizeof(float)   * *event.no_hits;
  event.hit_Zs           = (int32_t*)input; input += sizeof(int32_t) * *event.no_hits;

  if (input != end)
    throw std::runtime_error("failed to deserialize event");
}

/**
 * Combine all solutions, in the form:
 *
 * int numberOfEvents
 * per event:
 *     int size
 *
 * per event:
 *     char* output
 *
 * @param solutions
 * @param output
 */
void mergeSolutions(const std::vector<std::vector<char> >& solutions, std::vector<char>& output) {
    int numberOfEvents = solutions.size();
    output.resize((numberOfEvents + 1) * sizeof(int));
    char* outputPointer = ((char*) &(output[0]));

    memcpy(outputPointer, &numberOfEvents, sizeof(int));
    outputPointer += sizeof(int);

    int return_size = 0;
    for (int i = 0; i < solutions.size(); ++i) {
        // Requires lvalue
        int solutions_size = solutions[i].size();
        return_size += solutions_size;
        memcpy(outputPointer, &solutions_size, sizeof(int));
    }

    // After resizing, the pointer may change, so recalculate
    output.resize(output.size() + return_size);
    outputPointer = ((char*) &(output[0])) + (numberOfEvents + 1) * sizeof(int);

    for (int i = 0; i < solutions.size(); ++i) {
        memcpy(outputPointer, &(solutions[i][0]), solutions[i].size());
        outputPointer += solutions[i].size();
    }
}
