#include "logical_instructions.h"


namespace emulator
{
    void invl(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Logical_inv_instruction>(instruction);
        auto& reg1 = state.registers[instr.r];
        reg1 = ~reg1;
    }
}