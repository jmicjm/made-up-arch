#include "processor.h"
#include "control_flow_instructions.h"
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
    state.registers[Processor_state::program_counter] = state.interruptVector().reset;
}

void emulator::Processor::executeNext()
{
    auto& program_counter = state.registers[Processor_state::program_counter];
    if (program_counter + sizeof(Instruction_t) <= state.memory.size())
    {
        const auto instruction = reinterpret_cast<Instruction_t&>(state.memory[program_counter]);
        program_counter += sizeof(Instruction_t);

        if (const auto op = decoder.decode(instruction))
        {    
            op(state, instruction);       
        }
        else branch(state, state.interruptVector().invalid_opcode, true);
    }
    else branch(state, state.interruptVector().invalid_address, true);
}
