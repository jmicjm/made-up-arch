#pragma once
#include "timer.h"
#include "../address_range.h"
#include <cstdint>


namespace emulator
{
    constexpr Address_range timer0_address = { 0xFFFF'FFFF'FFFF'0000, 0xFFFF'FFFF'FFFF'0000 + sizeof(Timer_state) };
    constexpr Address_range timer1_address = { timer0_address.end, timer0_address.end + sizeof(Timer_state) };

    struct Peripherals
    {
        Timer timer0{timer0_address.begin};
        Timer timer1{timer1_address.begin};

        void update(Processor_state& state);
    };
}