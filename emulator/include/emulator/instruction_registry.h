#pragma once
#include "processor_state.h"
#include "instruction.h"
#include "instruction_decoder.h"


namespace emulator
{
    class Instruction_registry
    {
        template<Instruction_t>
        friend class Register_instruction;

        static inline Instruction_map instructions;
    public:
        static const Instruction_map& getInstructions() { return instructions; }
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
