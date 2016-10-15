gaudi_subdir(CpService v0r1)

gaudi_depends_on_subdirs(
  CpManager/CpIpc)

gaudi_add_module(
  CpService
  src/*.cpp
  INCLUDE_DIRS CpIpc
  LINK_LIBRARIES CpIpc GaudiKernel)

gaudi_install_headers(CpService)
