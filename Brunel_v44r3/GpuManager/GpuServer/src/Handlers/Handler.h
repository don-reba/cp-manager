// Definitions for handlers.

#pragma once

#include <stdint.h>
#include <vector>

typedef std::vector<uint8_t> Data;

typedef void * AllocParam;
typedef void * (*Alloc)(std::size_t size, AllocParam param);

typedef void (*Handler)(
   const Data & data,
   Alloc        resultAlloc,
   AllocParam   resultAllocParam);
