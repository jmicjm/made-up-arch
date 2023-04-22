#include "logical_instructions.h"
#include "impl_common.h"
#include <functional>
#include <type_traits>
#include <bit>


namespace emulator
{
    template<typename op>
    void logical3regInstruction(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Logical_instruction>(instruction);

        auto& rdst = state.registers[instr.rdst];
        const auto r1 = state.registers[instr.r1];
        const auto r2 = state.registers[instr.r2];

        rdst = op{}(r1, r2);

        updateNZflags(state, rdst);
    }


    void andl(Processor_state& state, Instruction_t instruction)
    {
        logical3regInstruction<std::bit_and<>>(state, instruction);
    }

    void orl(Processor_state& state, Instruction_t instruction)
    {
        logical3regInstruction<std::bit_or<>>(state, instruction);
    }

    void xorl(Processor_state& state, Instruction_t instruction)
    {
        logical3regInstruction<std::bit_xor<>>(state, instruction);
    }

    void invl(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Logical_inv_instruction>(instruction);
        auto& rdst = state.registers[instr.rdst];
        const auto rsrc = state.registers[instr.rsrc];

        rdst = ~rsrc;

        updateNZflags(state, rdst);
    }

    void shift(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Shift_instruction>(instruction);

        auto& rdst = state.registers[instr.rdst];
        const auto r1 = state.registers[instr.r1];
        const auto shift = instr.imm_arg ? instruction_cast<Shift_imm_instruction>(instruction).imm : state.registers[instr.r2] & (1ull << 6) - 1;

        switch (instr.type)
        {
        case Shift_type::logical_left:
        case Shift_type::arithmetic_left:
            rdst = r1 << shift;
            break;
        case Shift_type::logical_right:
            rdst = r1 >> shift;
            break;
        case Shift_type::arithmetic_right:
            rdst = static_cast<std::make_signed_t<decltype(r1)>>(r1) >> shift;
        }

        if (shift > 0)
        {
            switch (instr.type)
            {
            case Shift_type::logical_left:
            case Shift_type::arithmetic_left:
                state.pswFields().carry = static_cast<bool>(r1 & (1ull << sizeof(rdst) * 8 - shift));
                break;
            case Shift_type::logical_right:
            case Shift_type::arithmetic_right:
                state.pswFields().carry = static_cast<bool>(r1 & (1ull << shift - 1));
            }
        }

        updateNZflags(state, rdst);
    }

    void rotate(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Rotate_instruction>(instruction);

        auto& rdst = state.registers[instr.rdst];
        const auto r1 = state.registers[instr.r1];
        const auto offset = instr.imm_arg ? instruction_cast<Rotate_imm_instruction>(instruction).imm : state.registers[instr.r2] & (1ull << 6) - 1;

        switch (instr.type)
        {
        case Rotate_type::left:
            rdst = std::rotl(r1, offset);
            break;
        case Rotate_type::right:
            rdst = std::rotr(r1, offset);
        }

        if (offset > 0)
        {
            switch (instr.type)
            {
            case Rotate_type::left:
                state.pswFields().carry = static_cast<bool>(r1 & (1ull << sizeof(rdst) * 8 - offset));
                break;
            case Rotate_type::right:
                state.pswFields().carry = static_cast<bool>(r1 & (1ull << offset - 1));
            }
        }

        updateNZflags(state, rdst);
    }
}