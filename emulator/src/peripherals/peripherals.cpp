#include "peripherals/peripherals.h"
#include "processor_state.h"
#include "../rw_handlers.h"


namespace emulator
{
    void Peripherals::update(Processor_state& state)
    {
        timer0.update(state);
        timer1.update(state);
    }

    REGISTER_WRITE_HANDLER(timer0_address_range, [](Processor_state& state, auto... args) { state.peripherals.timer0.write(state, args...); })
    REGISTER_READ_HANDLER(timer0_address_range, [](Processor_state& state, auto... args) { state.peripherals.timer0.read(state, args...); })

    REGISTER_WRITE_HANDLER(timer1_address_range, [](Processor_state& state, auto... args) { state.peripherals.timer1.write(state, args...); })
    REGISTER_READ_HANDLER(timer1_address_range, [](Processor_state& state, auto... args) { state.peripherals.timer1.read(state, args...); })
}