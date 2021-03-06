#pragma once

#include "cuda_runtime.h"
#include "device_launch_parameters.h"

#include <cfloat>
#include <cmath>
#include <cstdint>
#include <cstdio>
#include <cstdlib>

#include <vector>

#define NUMTHREADS_X 64
#define MAX_NUMTHREADS_Y 16
#define NUM_ATOMICS 5
#define USE_SHARED_FOR_HITS 1
#define SH_HIT_MULT 2

#define MAX_TRACKS 3000
#define MAX_TRACK_SIZE 24

#define REQUIRED_UNIQUES 0.6f
#define MIN_HITS_TRACK 3
#define MAX_FLOAT FLT_MAX
#define MIN_FLOAT -FLT_MAX
#define MAX_SKIPPED_MODULES 3
#define TTF_MODULO 2000

#define PARAM_W 3966.94f // 0.050 / sqrt( 12. )
#define PARAM_MAXXSLOPE 0.4f
#define PARAM_MAXYSLOPE 0.3f

#define PARAM_TOLERANCE_BASIC 0.15f
#define PARAM_TOLERANCE_EXTENDED 0.3f
#define PARAM_TOLERANCE_EXTRA 0.6f

#define PARAM_TOLERANCE PARAM_TOLERANCE_EXTRA

#define MAX_SCATTER 0.000016f
#define SENSOR_DATA_HITNUMS 3

#define PRINT_SOLUTION false
#define ASSERTS_ENABLED false

#if ASSERTS_ENABLED == true
#include "assert.h"
#define ASSERT(EXPR) assert(EXPR);
#else
#define ASSERT(EXPR)
#endif

typedef std::vector<std::uint8_t> Data;

struct Sensor {
  unsigned int hitStart;
  unsigned int hitNums;

  __device__ Sensor(){}
  __device__ Sensor(const int _hitStart, const int _hitNums) :
    hitStart(_hitStart), hitNums(_hitNums) {}
};

struct Hit {
  float x;
  float y;
  float z;

  __device__ Hit(){}
  __device__ Hit(const float _x, const float _y, const float _z) :
    x(_x), y(_y), z(_z) {}
};

struct Track { // 4 + 24 * 4 = 100 B
  uint32_t hitsNum;
  uint32_t hits[MAX_TRACK_SIZE];

  __device__ Track(){}
  __device__ Track(const unsigned int hitsNum, unsigned int h0, unsigned int h1, unsigned int h2) :
    hitsNum(hitsNum) {
      hits[0] = h0;
      hits[1] = h1;
      hits[2] = h2;
    }
};

struct Tracklet {
  unsigned int hitsNum;
  unsigned int h0, h1, h2, hnext;

  __device__ Tracklet(){}
  __device__ Tracklet(const unsigned int _hitsNum, unsigned int _h0, unsigned int _h1, unsigned int _h2) :
    hitsNum(_hitsNum), h0(_h0), h1(_h1), h2(_h2) {}
};
