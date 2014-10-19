#pragma once

#include "CudaException.h"

#include "cuda_runtime.h"

#include <cstring>
#include <exception>
#include <iostream>
#include <vector>
#include <sstream>
#include <stdint.h>

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

void setHPointersFromInput(uint8_t * input, size_t size);
void mergeSolutions(const std::vector<std::vector<char> >& solutions, std::vector<char>& output);
