#pragma once
#include "../processor_state.fwd.h"
#include <cstdint>


namespace emulator
{
    struct Timer_state
    {
        enum Mode : uint8_t
        {
            forward,
            backward
        };

        uint64_t value = 0;
        uint64_t reset_value = 0;

        struct Config_word
        {
            uint64_t enable : 1 = false;
            uint64_t interrupt_enable : 1 = false;
            uint64_t mode : 1 = Mode::forward;
            uint64_t auto_reload : 1 = false;
            uint64_t overflow : 1 = false;

        private:
            uint64_t reserved : 59 = 0;
        } config_word;
    };

    class Timer
    {
        uint64_t address;
        uint64_t interrupt_idx;

    public:
        Timer_state state;

        Timer(uint64_t address, uint64_t interrupt_idx);

        void update(Processor_state& state);

        bool read(Processor_state& state, uint64_t address, uint8_t* dst, uint8_t size) const;
        bool write(Processor_state& state, uint64_t address, uint8_t* src, uint8_t size);
    };
}