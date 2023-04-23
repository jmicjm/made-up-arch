#include "peripherals/peripherals.h"
#include "processor_state.h"


void emulator::Peripherals::update(Processor_state& state)
{
    timer0.update(state);
    timer1.update(state);
}