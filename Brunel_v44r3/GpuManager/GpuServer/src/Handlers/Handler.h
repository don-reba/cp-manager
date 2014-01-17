// Definitions for handlers.

#pragma once

#include <stdint.h>
#include <vector>

// Data is processed in batches. A batch consists of input data sets
// from multiple clients.

typedef std::vector<uint8_t>     Data;
typedef std::vector<const Data*> Batch;

// Allocator for the results of the compuation. It takes:
// - the batch member index to which the result applies
// - size of the result
// - an opaque parameter given to the handler

typedef void * AllocParam;
typedef void * (*Alloc)(std::size_t index, std::size_t size, AllocParam param);

// The handler is a function that takes a batch of input data sets
// and computes the results for each. This approach enables taking
// advantage of high-throughput high-latency processors.

typedef void (*Handler)(
   const Batch & batch,
   Alloc         resultAlloc,
   AllocParam    resultAllocParam);
