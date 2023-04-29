cmake_minimum_required (VERSION 3.12)


add_library (emulator 
    "src/processor.cpp" 
    "src/processor_state.cpp" 
    "src/instruction_decoder.cpp"
    "src/arithmetic_instructions.cpp"
    "src/data_instructions.cpp"
    "src/logical_instructions.cpp"
    "src/control_flow_instructions.cpp"
    "src/comparison_instructions.cpp"
    "src/memory_helpers.cpp"
    "src/memory_templates.cpp"
    "src/peripherals/timer.cpp"
    "src/peripherals/peripherals.cpp"
    "src/debugging/human_readable_instruction.cpp"
)

target_include_directories(emulator PRIVATE "include/emulator")
set_target_properties(emulator PROPERTIES
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF
)

if(BUILD_TESTING)
    add_subdirectory(tests)
endif()
