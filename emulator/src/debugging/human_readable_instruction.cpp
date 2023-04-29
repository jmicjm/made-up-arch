#include "debugging/human_readable_instruction.h"


std::string emulator::debugging::instructionToString(Instruction_t instruction)
{
    return InstructionDescribers::describers[opcode(instruction)](instruction);
}
