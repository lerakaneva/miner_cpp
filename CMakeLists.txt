cmake_minimum_required(VERSION 3.6)
project(mining)

# Update the submodules here
include(cmake/UpdateSubmodules.cmake)
# Enable testing for this project
include(CTest)

add_subdirectory(blocks)
add_subdirectory(byte_arrays)
add_subdirectory(crypto)
add_subdirectory(external)

add_executable(mining main.cpp)

add_library(cxx_setup INTERFACE)
target_compile_options(cxx_setup
  INTERFACE -Wall -Wpedantic -Wextra)
target_compile_features(cxx_setup INTERFACE cxx_std_20)
target_include_directories(cxx_setup
  INTERFACE ${PROJECT_SOURCE_DIR})

target_link_libraries(mining PRIVATE
    blocks_lib
    cxx_setup
    -lm
    -ldl
    )
