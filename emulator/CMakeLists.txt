cmake_minimum_required (VERSION 3.12)


add_library (emulator 
    "src/processor.cpp" 
    "src/processor_state.cpp" 
    "src/instruction_decoder.cpp"
    "src/arithmetic_instructions.cpp"
    "src/data_instructions.cpp"
    "src/logical_instructions.cpp"
)

target_include_directories(emulator PRIVATE "include/emulator")
set_property(TARGET emulator PROPERTY CXX_STANDARD 20)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
