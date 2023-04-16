#pragma once
#include "instruction.h"
#include <cstdint>
#include <vector>


namespace emulator
{
    void writeCode(std::vector<uint8_t>& memory, uint64_t address, const std::vector<Instruction_t>& code);
}