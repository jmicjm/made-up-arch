cmake_minimum_required (VERSION 3.12)

project ("made-up-arch")

include(CTest)

add_executable(example "example.cpp")
target_include_directories(example PRIVATE "emulator/include")
set_target_properties(example PROPERTIES
    CXX_STANDARD 20
    CXX_EXTENSIONS OFF
)

add_subdirectory ("emulator")

target_link_libraries(example PRIVATE emulator)
