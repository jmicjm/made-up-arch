#pragma once
#include "processor_state.h"
#include "instruction.h"
#include <unordered_map>


namespace emulator
{
    class Instruction_registry
    {
        friend class Instruction_decoder;
        template<Instruction_t>
        friend class Register_instruction;

        static inline std::unordered_map<Instruction_t, void (*)(Processor_state&, Instruction_t)> instructions;
    };

    template<Instruction_t opcode>
    class Register_instruction {};
}

#define REGISTER_INSTRUCTION(opcode, instr)                                 \
template<>                                                                  \
class Register_instruction<opcode>                                          \
{                                                                           \
    struct Register                                                         \
    {                                                                       \
        Register()                                                          \
        {                                                                   \
            Instruction_registry::instructions.insert({ opcode, instr });   \
        }                                                                   \
    };                                                                      \
    inline static auto dummy = Register{};                                  \
};                                                                          
