#include "logical_instructions.h"


namespace emulator
{
    void invl(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Logical_inv_instruction>(instruction);
        auto& rdst = state.registers[instr.rdst];
        const auto rsrc = state.registers[instr.rsrc];
        rdst = ~rsrc;
    }
}