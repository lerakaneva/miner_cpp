cmake -S . -B build
cmake --build build -j 12
./build/mining

run tests: 
GTEST_COLOR=1 ctest --test-dir build --output-on-failure -j12