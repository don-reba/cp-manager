#pragma once

#include "Handler.h"

namespace Handlers {

  void tripletSearchGPU(
      const Batch & batch,
      Alloc         allocResult,
      AllocParam    allocResultParam);

}
