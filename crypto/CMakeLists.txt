find_package(OpenSSL REQUIRED)

add_library(crypto crypto.cpp)
target_link_libraries(crypto PUBLIC cxx_setup)
target_link_libraries(crypto PRIVATE OpenSSL::SSL)