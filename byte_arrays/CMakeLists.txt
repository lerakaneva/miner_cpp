add_library(byte_arrays byte_arrays.cpp)
target_link_libraries(byte_arrays PUBLIC cxx_setup)
target_link_libraries(byte_arrays PRIVATE crypto)

if (BUILD_TESTING)
    add_executable(byte_arrays_ut byte_arrays_ut.cpp)
    target_link_libraries(byte_arrays_ut PRIVATE byte_arrays GTest::gtest_main)
    include(GoogleTest)
    # Finds all the Google tests associated with the executable
    gtest_discover_tests(byte_arrays_ut)
endif()