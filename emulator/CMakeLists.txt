cmake_minimum_required (VERSION 3.8)

add_library (emulator 
    "processor.cpp" 
    "processor_state.cpp" 
    "instruction_decoder.cpp"
    "arithmetic_instructions.cpp"
    "data_instructions.cpp"
  )

set_property(TARGET emulator PROPERTY CXX_STANDARD 20)
