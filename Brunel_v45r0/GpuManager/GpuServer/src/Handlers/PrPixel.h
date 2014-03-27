#pragma once

#include "Handler.h"

namespace Handlers {

  void searchByPair(
      const Batch & batch,
      Alloc         allocResult,
      AllocParam    allocResultParam);

}
