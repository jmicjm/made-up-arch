#pragma once
#include "instruction.h"
#include "processor_state.h"
#include <vector>


namespace emulator
{
    class Instruction_decoder
    {
        using Instruction_f = void(Processor_state&, Instruction_t);

        std::vector<Instruction_f*> instructions;

    public:
        Instruction_decoder();

        Instruction_f* decode(Instruction_t instruction);
    };
}
