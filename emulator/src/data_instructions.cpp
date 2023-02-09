#include "data_instructions.h"
#include <cstdlib>


namespace emulator
{
    void mov(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Mov_instruction>(instruction);
        auto& reg_dst = state.registers[instr.rdst];

        if (instr.imm_arg)
        {
            reg_dst = instruction_cast<Mov_imm_instruction>(instruction).imm;
        }
        else
        {
            reg_dst = state.registers[instr.rsrc];
        }
    }

    void ldr(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Ldr_instruction>(instruction);
        auto& reg_dst = state.registers[instr.rdst];

        const uint64_t addr = state.registers[instr.rbase] + instr.off;

        if (addr + (1ull << instr.size) <= state.memory.size())
        {
            switch (instr.size)
            {
            case Data_type::byte:
                reg_dst = state.memory[addr];
                break;
            case Data_type::doublebyte:
                reg_dst = reinterpret_cast<uint16_t&>(state.memory[addr]);
                break;
            case Data_type::quadbyte:
                reg_dst = reinterpret_cast<uint32_t&>(state.memory[addr]);
                break;
            case Data_type::octobyte:
                reg_dst = reinterpret_cast<uint64_t&>(state.memory[addr]);
            }   
        }
        else std::exit(-1);
    }

    void str(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Str_instruction>(instruction);
        const auto reg_src = state.registers[instr.rsrc];

        const uint64_t addr = state.registers[instr.rbase] + instr.off;

        if (addr + (1ull << instr.size) <= state.memory.size())
        {
            switch (instr.size)
            {
            case Data_type::byte:
                state.memory[addr] = reg_src;
                break;
            case Data_type::doublebyte:
                reinterpret_cast<uint16_t&>(state.memory[addr]) = reg_src;
                break;
            case Data_type::quadbyte:
                reinterpret_cast<uint32_t&>(state.memory[addr]) = reg_src;
                break;
            case Data_type::octobyte:
                reinterpret_cast<uint64_t&>(state.memory[addr]) = reg_src;
            }
        }
        else std::exit(-1);
    }
}
