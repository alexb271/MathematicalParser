cmake_minimum_required(VERSION 3.22)

project(Parser)

# output to the root of the build directory
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# set C standard
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED True)

# export flags for YCM
set(CMAKE_EXPORT_COMPILE_COMMANDS on)

# add backend: Interpreter library
add_subdirectory(${PROJECT_SOURCE_DIR}/src/backend/)

# add frontend
add_subdirectory(${PROJECT_SOURCE_DIR}/src/frontend/)

# add test
add_subdirectory(${PROJECT_SOURCE_DIR}/unittest)

# to build with debug flags, use:
# cmake -DCMAKE_BUILD_TYPE=Debug ..
#
# other flags: Release, RelWithDebInfo, MinSizeRel
