#include "data_instructions.h"
#include "memory_bus.h"
#include "control_flow_instructions.h"
#include <type_traits>


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


    template<template<typename> typename f, template<typename> typename type_projection = std::type_identity_t>
    void dispatchByDataType(uint8_t data_type, auto&&... args)
    {
        switch (data_type)
        {
        case Data_type::byte:
            f<type_projection<uint8_t>>{}(args...);
            break;
        case Data_type::doublebyte:
            f<type_projection<uint16_t>>{}(args...);
            break;
        case Data_type::quadbyte:
            f<type_projection<uint32_t>>{}(args...);
            break;
        case Data_type::octobyte:
            f<type_projection<uint64_t>>{}(args...);
        }
    }
    
    template<typename T>
    struct Load_register
    {    
        void operator()(Processor_state& state, uint8_t reg, uint64_t address)
        {
            if (auto mem = readMemory<T>(state, address))
            {
                state.registers[reg] = *mem;
            }
        };
    };

    void ldr(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Ldr_instruction>(instruction);
        const uint64_t addr = state.registers[instr.rbase] + instr.off;

        if (instr.sign_extend) dispatchByDataType<Load_register, std::make_signed_t>(instr.size, state, instr.rdst, addr);
        else dispatchByDataType<Load_register>(instr.size, state, instr.rdst, addr);
    }


    template<typename T>
    struct Store_register
    {
        void operator()(Processor_state& state, uint8_t reg, uint64_t address)
        {
            writeMemory<T>(state, address, state.registers[reg]);
        };
    };

    void str(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Str_instruction>(instruction);
        const uint64_t addr = state.registers[instr.rbase] + instr.off;

        dispatchByDataType<Store_register>(instr.size, state, instr.rsrc, addr);
    }


    void push(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Push_instruction>(instruction);
        
        state.registers[Processor_state::stack_pointer] -= 1ull << instr.size;

        str(state, toInstruction(Str_instruction{ .size = instr.size, .rsrc = instr.rsrc, .rbase = Processor_state::stack_pointer, .off = 0 }));
    }

    void pop(Processor_state& state, Instruction_t instruction)
    {
        const auto instr = instruction_cast<Pop_instruction>(instruction);

        ldr(state, toInstruction(Ldr_instruction{ .size = instr.size, .sign_extend = instr.sign_extend, .rdst = instr.rdst, .rbase = Processor_state::stack_pointer, .off = 0 }));

        state.registers[Processor_state::stack_pointer] += 1ull << instr.size;
    }
}
