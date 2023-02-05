#include "instruction_decoder.h"
#include "instructions.h"


emulator::Instruction_decoder::Instruction_decoder() : instructions(256, nullptr)
{
    instructions[Opcode::add] = add;
    instructions[Opcode::sub] = sub;
    instructions[Opcode::mov] = mov;
    instructions[Opcode::ldr] = ldr;
    instructions[Opcode::str] = str;
    instructions[Opcode::andl] = andl;
    instructions[Opcode::orl] = orl;
    instructions[Opcode::xorl] = xorl;
    instructions[Opcode::invl] = invl;
}

auto emulator::Instruction_decoder::decode(Instruction_t instruction) -> Instruction_f*
{
    return instructions[instruction & 0xFF];
}
