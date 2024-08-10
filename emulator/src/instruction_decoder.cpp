#include "instruction_decoder.h"
#include "instructions.h"
#include "instruction.h"


emulator::Instruction_decoder::Instruction_decoder(const Instruction_map& inst) : instructions(1 << opcode_size, nullptr)
{
    for (const auto& [opcode, instr] : inst)
    {
        if (opcode < instructions.size()) instructions[opcode] = instr;
    }
}

auto emulator::Instruction_decoder::decode(Instruction_t instruction) const -> Instruction_f*
{
    return instructions[opcode(instruction)];
}
