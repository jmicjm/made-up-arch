#include "processor.h"
#include <cstdlib>
#include <utility>


emulator::Processor::Processor(std::vector<uint8_t> memory)
{
    state.memory = std::move(memory);
    reset();
}

emulator::Processor::Processor(size_t memory_size)
{
    state.memory.resize(memory_size);
}

void emulator::Processor::reset()
{
    state.registers = Processor_state{}.registers;
    state.status_word = Processor_state{}.status_word;
    state.registers[Processor_state::program_counter] = interruptVector().reset;
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
        }
        else
        {
            state.registers[Processor_state::link_register] = state.registers[Processor_state::program_counter];
            state.registers[Processor_state::program_counter] = interruptVector().invalid_opcode;
        }
    }
    else std::exit(-1);
}

Interrupt_vector& emulator::Processor::interruptVector()
{
    return const_cast<Interrupt_vector&>(std::as_const(*this).interruptVector());
}

const Interrupt_vector& emulator::Processor::interruptVector() const
{
    return reinterpret_cast<const Interrupt_vector&>(state.memory[0]);
}
