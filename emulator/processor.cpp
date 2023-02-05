#include "processor.h"
#include <cstdlib>

emulator::Processor::Processor(size_t memory_size)
{
    state.memory.resize(memory_size);
}

void emulator::Processor::executeNext()
{
    const uint64_t program_counter = state.registers[Processor_state::program_counter];
    if (program_counter + 7 < state.memory.size())
    {
        const uint64_t instruction = reinterpret_cast<uint64_t&>(state.memory[program_counter]);
        if (const auto op = decoder.decode(instruction))
        {
            state.registers[Processor_state::program_counter] += 8;
            op(state, instruction);       
            return;
        }
    }

    std::exit(-1);
}
