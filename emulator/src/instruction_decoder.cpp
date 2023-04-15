#include "instruction_decoder.h"
#include "instructions.h"
#include "instruction.h"
#include "instruction_registry.h"


emulator::Instruction_decoder::Instruction_decoder() : instructions(1 << opcode_size, nullptr)
{
    for (const auto& [opcode, instr] : Instruction_registry::instructions)
    {
        if (opcode < instructions.size()) instructions[opcode] = instr;
    }
}

auto emulator::Instruction_decoder::decode(Instruction_t instruction) const -> Instruction_f*
{
    return instructions[opcode(instruction)];
}
