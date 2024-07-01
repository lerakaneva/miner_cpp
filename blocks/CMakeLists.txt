add_library(blocks_lib blocks.cpp)
target_link_libraries(blocks_lib PUBLIC cxx_setup)
target_link_libraries(blocks_lib PRIVATE byte_arrays)
target_link_libraries(blocks_lib PRIVATE crypto)