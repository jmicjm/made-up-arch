cmake_minimum_required (VERSION 3.12)

project ("made-up-arch")

include(CTest)

add_executable(main "main.cpp")
target_include_directories(main PRIVATE "emulator/include")
set_target_properties(main PROPERTIES
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF
)

add_subdirectory ("emulator")

target_link_libraries(main PRIVATE emulator)
