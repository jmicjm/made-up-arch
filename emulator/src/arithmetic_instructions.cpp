#include "arithmetic_instructions.h"
#include "common.h"
#include <limits>
#include <type_traits>


namespace emulator
{
    void add(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Arithmetic_instruction>(instruction);

        auto& rdst = state.registers[instr.rdst];
        const auto r1 = state.registers[instr.r1];
        const auto r2 = instr.imm_arg ? instruction_cast<Arithmetic_imm_instruction>(instruction).imm : state.registers[instr.r2];

        rdst = r1 + r2;

        updateNZflags(state, rdst);
        state.pswFields().carry = r1 > std::numeric_limits<std::remove_reference_t<decltype(rdst)>>::max() - r2;
        state.pswFields().overflow = msb(r1) == msb(r2) && msb(r1) != msb(rdst);
    }

    void sub(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Arithmetic_instruction>(instruction);

        auto& rdst = state.registers[instr.rdst];
        const auto r1 = state.registers[instr.r1];
        const auto r2 = instr.imm_arg ? instruction_cast<Arithmetic_imm_instruction>(instruction).imm : state.registers[instr.r2];

        rdst = r1 - r2;

        updateSubFlags(state, rdst, r1, r2);
    }
}