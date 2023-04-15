#pragma once
#include "interrupt_vector.h"
#include "instructions.h"
#include "processor_state.h"
#include "common.h"
#include <cstdint>
#include <vector>
#include <utility>
#include <cstring>


namespace emulator
{
    inline std::pair<std::vector<uint8_t>, uint64_t> generateBasicMemory(size_t size, uint64_t initial_sp)
    {
        std::vector<uint8_t> memory(size);
        Interrupt_vector& int_vec = reinterpret_cast<Interrupt_vector&>(memory[0]);
        int_vec.reset = sizeof(Interrupt_vector);
        int_vec.invalid_opcode = 0x0;

        std::vector<Instruction_t> reset_handler;
        reset_handler.push_back(toInstruction(Ldr_instruction{ .size = Data_type::octobyte, .rdst = Processor_state::stack_pointer, .rbase = Processor_state::program_counter, .off = sizeof(Instruction_t) }));
        reset_handler.push_back(toInstruction(Branch_instruction{ .link = 0, .condition = Branch_condition::always, .offset = 2}));
        reset_handler.push_back(lowerHalf(initial_sp));
        reset_handler.push_back(upperHalf(initial_sp));

        std::memcpy(memory.data() + int_vec.reset, reset_handler.data(), reset_handler.size() * sizeof(Instruction_t));

        return { memory, int_vec.reset + reset_handler.size() * sizeof(Instruction_t) };
    }
}