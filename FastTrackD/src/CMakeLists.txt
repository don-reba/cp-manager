cmake_minimum_required(VERSION 2.6)
project(fasttrackd)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)
set(LIB_INPUT_PATH ${PROJECT_SOURCE_DIR}/../lib)

add_executable(
    fasttrackd
    App.cpp
    Main.cpp
    Protocol.cpp
    SocketServer.cpp
    SystemException.cpp
    )

set_target_properties(
    fasttrackd
    PROPERTIES COMPILE_FLAGS "-ffor-scope -Werror -Wall -O3 -g"
    )

find_package(Boost REQUIRED program_options)
find_package(Doxygen)

include_directories(/usr/include/glib-2.0)

target_link_libraries(fasttrackd ${Boost_PROGRAM_OPTIONS_LIBRARY})

if (DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  add_custom_target(doc ALL
      ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen" VERBATIM
      )
endif(DOXYGEN_FOUND)
