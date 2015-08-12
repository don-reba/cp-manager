gaudi_subdir(GpuAlgorithm v0r1)

gaudi_depends_on_subdirs(
  GpuManager/GpuService)

gaudi_add_module(
  GpuAlgorithm
  src/*.cpp
  INCLUDE_DIRS GpuService
  LINK_LIBRARIES GaudiAlgLib)
