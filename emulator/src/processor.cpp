#include "processor.h"
#include <cstdlib>

emulator::Processor::Processor(size_t memory_size)
{
    state.memory.resize(memory_size);
}

void emulator::Processor::executeNext()
{
    const uint64_t program_counter = state.registers[Processor_state::program_counter];
    if (program_counter + sizeof(Instruction_t) <= state.memory.size())
    {
        const Instruction_t instruction = reinterpret_cast<Instruction_t&>(state.memory[program_counter]);
        if (const auto op = decoder.decode(instruction))
        {
            state.registers[Processor_state::program_counter] += sizeof(Instruction_t);
            op(state, instruction);       
            return;
        }
    }

    std::exit(-1);
}
