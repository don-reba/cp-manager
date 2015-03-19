#pragma once

#include "CudaException.h"
#include "Definitions.h"

#include <vector>
#include <sstream>


#define cudaCheck(stmt) {              \
    cudaError_t err = stmt;            \
    if (err != cudaSuccess)            \
      throw CudaException(err, #stmt); \
}

#define cudaCheckLast(msg)                        \
  if (cudaPeekAtLastError() != cudaSuccess) {     \
    throw CudaException(cudaGetLastError(), msg); \
  }

template <class T>
std::string toString(T t){
    std::stringstream ss;
    std::string s;
    ss << t;
    ss >> s;
    return s;
}

void mergeSolutions(const std::vector<std::vector<char> >& solutions, std::vector<char>& output);
