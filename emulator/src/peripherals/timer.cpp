#include "peripherals/timer.h"
#include "processor_state.h"
#include "control_flow_instructions.h"
#include "common.h"
#include <limits>
#include <cstring>


emulator::Timer::Timer(uint64_t interrupt_idx) 
    : interrupt_idx{interrupt_idx} {}

void emulator::Timer::update(Processor_state& proc_state)
{
    using enum Timer_state::Mode;

    if (state.config_word.enable)
    {
        const auto handleOverflow = [&]
        {
            state.config_word.overflow = true;
            if (!state.config_word.auto_reload) state.config_word.enable = false;

            if (state.config_word.interrupt_enable)
            {
                const auto interrupt_handler_address = proc_state.interruptVector().handlers[interrupt_idx];
                branchInterrupt(proc_state, interrupt_handler_address);
            }
        };

        switch (state.config_word.mode)
        {
        case forward:
            if (state.value == state.reset_value)
            {
                state.value = 0;
                handleOverflow();
            }
            else state.value++;
            break;
        case backward:
            if (state.value == 0)
            {
                state.value = state.reset_value;
                handleOverflow();
            }
            else state.value--;
        }
    }
}

bool emulator::Timer::read(Processor_state& state, uint64_t offset, uint8_t* dst, uint8_t size) const
{
    if (offset + size > sizeof(Timer_state)) return false;

    std::memcpy(dst, reinterpret_cast<const uint8_t*>(&this->state) + offset, size);

    return true;
}

bool emulator::Timer::write(Processor_state& state, uint64_t offset, uint8_t* src, uint8_t size)
{
    if (offset + size > sizeof(Timer_state)) return false;

    std::memcpy(reinterpret_cast<uint8_t*>(&this->state) + offset, src, size);

    return true;
}
