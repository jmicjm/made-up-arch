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
        constexpr static auto link_register = register_count - 3;

        std::array<uint64_t, register_count> registers{};

        struct [[gnu::may_alias]] Status_word
        {
            uint8_t carry : 1;
            uint8_t overflow : 1;
            uint8_t negative : 1;
            uint8_t zero : 1;
        };

        uint8_t status_word = 0x0;

        std::vector<uint8_t> memory;

        Status_word& pswFields();
        const Status_word& pswFields() const;

        void print() const;
    };
}