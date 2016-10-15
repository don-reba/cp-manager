gaudi_subdir(CpDriver v0r1)

gaudi_depends_on_subdirs(
  CpManager/CpIpc)

find_package(Boost COMPONENTS filesystem program_options system thread)

gaudi_add_executable(
  cpdriver
  src/*.cpp
  INCLUDE_DIRS Boost CpIpc CpHandler
  LINK_LIBRARIES Boost CpIpc CpHandler GaudiPluginService)
