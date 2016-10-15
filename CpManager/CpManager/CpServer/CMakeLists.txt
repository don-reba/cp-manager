gaudi_subdir(CpServer v0r1)

gaudi_depends_on_subdirs(
  CpManager/CpIpc
  CpManager/CpHandler)

find_package(Boost COMPONENTS filesystem program_options system thread)

gaudi_add_executable(
  cpserver
  src/*.cpp
  INCLUDE_DIRS Boost CpIpc CpHandler
  LINK_LIBRARIES Boost CpIpc CpHandler GaudiPluginService)
