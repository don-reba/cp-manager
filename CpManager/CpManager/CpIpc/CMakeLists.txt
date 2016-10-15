gaudi_subdir(CpIpc v0r1)

find_package(Boost COMPONENTS system thread)

gaudi_add_library(
  CpIpc
  src/*.cpp
  INCLUDE_DIRS Boost CpIpc
  PUBLIC_HEADERS CpIpc
  LINK_LIBRARIES Boost)
