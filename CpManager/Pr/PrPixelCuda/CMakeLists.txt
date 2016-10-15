gaudi_subdir(PrPixelCuda v0r1)

gaudi_depends_on_subdirs(
  CpManager/CpHandler)


set(CUDA_PROPAGATE_HOST_FLAGS OFF)

gaudi_add_cuda_module(
  PrPixelCuda
  src/*.cpp src/*.cu
  INCLUDE_DIRS CpHandler CUDA
  LINK_LIBRARIES CpHandler CUDA)
