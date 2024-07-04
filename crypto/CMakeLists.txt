find_package(OpenSSL REQUIRED)

add_library(crypto crypto.cpp)
target_link_libraries(crypto PUBLIC cxx_setup)
target_link_libraries(crypto PRIVATE OpenSSL::SSL)

if (BUILD_TESTING)
    add_executable(crypto_ut crypto_ut.cpp)
    target_link_libraries(crypto_ut PRIVATE crypto GTest::gtest_main)
    include(GoogleTest)
    # Finds all the Google tests associated with the executable
    gtest_discover_tests(crypto_ut)
endif()