#pragma once
#include "instruction.h"
#include "processor_state.h"
#include <vector>
#include <unordered_map>


namespace emulator
{
    using Instruction_map = std::unordered_map<Instruction_t, void (*)(Processor_state&, Instruction_t)>;

    class Instruction_decoder
    {
        using Instruction_f = void(Processor_state&, Instruction_t);

        std::vector<Instruction_f*> instructions;

    public:
        Instruction_decoder(const Instruction_map& instructions);

        Instruction_f* decode(Instruction_t instruction) const;
    };
}
