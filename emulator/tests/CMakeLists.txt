cmake_minimum_required (VERSION 3.12)

find_package(GTest CONFIG REQUIRED)

add_executable (tests 
    "src/simple_sequences.cpp"
    "src/arithmetic_instructions.cpp" 
    "src/logical_instructions.cpp" 
    "src/data_instructions.cpp" 
    "src/control_flow_instructions.cpp" 
    "src/comparison_instructions.cpp"
    "src/interrupts.cpp"
    "src/peripherals/timers.cpp"
)

target_include_directories(tests PRIVATE "../include")
set_target_properties(tests PROPERTIES
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF
)

target_link_libraries(tests PRIVATE emulator GTest::gtest_main)

include(GoogleTest)
gtest_discover_tests(tests)
