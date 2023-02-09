cmake_minimum_required (VERSION 3.12)

find_package(GTest CONFIG REQUIRED)

add_executable (tests 
    "src/instruction_tests.cpp" 
    "src/simple_sequences.cpp"
)
target_include_directories(tests PRIVATE "../include")
set_property(TARGET tests PROPERTY CXX_STANDARD 20)

target_link_libraries(tests PRIVATE emulator GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)
