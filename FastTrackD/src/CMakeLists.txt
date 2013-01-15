cmake_minimum_required(VERSION 2.6)
project(fasttrackd)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)

add_executable(
    fasttrackd
    main.cpp
    App.cpp
    )

set_target_properties(
    fasttrackd
    PROPERTIES COMPILE_FLAGS "-ffor-scope -Werror -Wall -O3 -g"
    )

#find_package(Boost REQUIRED program_options)

#target_link_libraries(fasttrackd ${Boost_PROGRAM_OPTIONS_LIBRARY})
target_link_libraries(fasttrackd /usr/lib64/libboost_program_options.so)
