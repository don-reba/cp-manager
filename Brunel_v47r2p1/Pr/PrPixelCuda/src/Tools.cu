#include "Tools.cuh"

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
