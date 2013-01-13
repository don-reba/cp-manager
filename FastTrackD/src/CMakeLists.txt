cmake_minimum_required(VERSION 2.6)
project(fasttrackd)

set(EXECUTABLE_OUTPUT_PATH ${PROJECT_SOURCE_DIR}/../bin)

add_executable(
    fasttrackd
    main.cpp
    )

set_target_properties(
    fasttrackd
    PROPERTIES COMPILE_FLAGS "-ffor-scope -Werror -Wall -O3 -g"
    )
