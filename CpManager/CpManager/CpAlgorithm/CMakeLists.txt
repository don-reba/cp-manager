gaudi_subdir(CpAlgorithm v0r1)

gaudi_depends_on_subdirs(
  CpManager/CpService)

gaudi_add_module(
  CpAlgorithm
  src/*.cpp
  INCLUDE_DIRS CpService
  LINK_LIBRARIES GaudiAlgLib)
