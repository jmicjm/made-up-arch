#include "instruction_decoder.h"
#include "instructions.h"


emulator::Instruction_decoder::Instruction_decoder() : instructions(256, nullptr)
{
    instructions[static_cast<uint8_t>(Opcode::add)] = add;
    instructions[static_cast<uint8_t>(Opcode::sub)] = sub;
    instructions[static_cast<uint8_t>(Opcode::mov)] = mov;
    instructions[static_cast<uint8_t>(Opcode::ldr)] = ldr;
    instructions[static_cast<uint8_t>(Opcode::str)] = str;
    instructions[static_cast<uint8_t>(Opcode::andl)] = andl;
    instructions[static_cast<uint8_t>(Opcode::orl)] = orl;
    instructions[static_cast<uint8_t>(Opcode::xorl)] = xorl;
    instructions[static_cast<uint8_t>(Opcode::invl)] = invl;
}

auto emulator::Instruction_decoder::decode(Instruction_t instruction) -> Instruction_f*
{
    return instructions[instruction & 0xFF];
}
