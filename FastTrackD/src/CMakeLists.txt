cmake_minimum_required(VERSION 2.6)
project(fasttrackd)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)

add_executable(
    fasttrackd
    main.cpp
    App.cpp
		MessageQueue.cpp
    )

set_target_properties(
    fasttrackd
    PROPERTIES COMPILE_FLAGS "-ffor-scope -Werror -Wall -O3 -g"
    )

find_package(Boost REQUIRED program_options)
find_package(Doxygen)

target_link_libraries(fasttrackd ${Boost_PROGRAM_OPTIONS_LIBRARY})
#target_link_libraries(fasttrackd /usr/lib64/libboost_program_options.so)

if (DOXYGEN_FOUND)
  configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Doxyfile.in ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile @ONLY)
  add_custom_target(doc ALL
      ${DOXYGEN_EXECUTABLE} ${CMAKE_CURRENT_BINARY_DIR}/Doxyfile
      WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}
      COMMENT "Generating API documentation with Doxygen" VERBATIM
      )
endif(DOXYGEN_FOUND)
