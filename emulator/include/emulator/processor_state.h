#pragma once
#include <cstdint>
#include <array>
#include <vector>


namespace emulator
{
    struct Processor_state
    {
        constexpr static auto register_count = 16;
        constexpr static auto program_counter = register_count - 1;
        constexpr static auto stack_pointer = register_count - 2;

        std::array<uint64_t, register_count> registers{};

        std::vector<uint8_t> memory;

        void print() const;
    };
}