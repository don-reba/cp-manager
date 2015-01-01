#pragma once

#include "CudaException.h"

#include "cuda_runtime.h"

#include <cstring>
#include <exception>
#include <iostream>
#include <vector>
#include <sstream>
#include <stdint.h>

struct Event {
  int   * no_sensors;
  int   * no_hits;
  int   * sensor_Zs;
  int   * sensor_hitStarts;
  int   * sensor_hitNums;
  int   * hit_IDs;
  float * hit_Xs;
  float * hit_Ys;
  int   * hit_Zs;
};

#define cudaCheck(stmt) {              \
    cudaError_t err = stmt;            \
    if (err != cudaSuccess)            \
      throw CudaException(err, #stmt); \
}

template <class T>
std::string toString(T t){
    std::stringstream ss;
    std::string s;
    ss << t;
    ss >> s;
    return s;
}

void parseEvent(uint8_t * input, size_t size, /*OUT*/ Event & event);
void mergeSolutions(const std::vector<std::vector<char> >& solutions, std::vector<char>& output);
