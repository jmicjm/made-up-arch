#pragma once
#include "timer.h"
#include "../interrupt_vector.h"
#include "../address_range.h"
#include <cstdint>


namespace emulator
{
    constexpr Address_range timer0_address_range = { 0xFFFF'FFFF'FFFF'0000, 0xFFFF'FFFF'FFFF'0000 + sizeof(Timer_state) };
    constexpr Address_range timer1_address_range = { timer0_address_range.end, timer0_address_range.end + sizeof(Timer_state) };

    struct Peripherals
    {
        Timer timer0{ Interrupts::timer0 };
        Timer timer1{ Interrupts::timer1 };

        void update(Processor_state& state);
    };
}