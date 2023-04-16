#pragma once
#include "interrupt_vector.h"
#include "instructions.h"
#include "processor_state.h"
#include "common.h"
#include "memory_helpers.h"
#include <cstdint>
#include <vector>
#include <utility>


namespace emulator
{
    std::pair<std::vector<uint8_t>, uint64_t> basicMemoryImage(size_t size, uint64_t initial_sp);
    std::vector<Instruction_t> basicResetHandler(uint64_t initial_sp);
    std::vector<Instruction_t> nopHandler();
}