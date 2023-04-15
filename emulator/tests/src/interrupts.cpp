#include <gtest/gtest.h>
#include "emulator/processor.h"
#include "emulator/instructions.h"
#include "emulator/basic_memory_template.h"
#include <vector>
#include <utility>
#include <cstdint>


using namespace emulator;

TEST(interrupts, reset)
{
    const auto initial_sp = 512;
    auto [memory, start_addr] = generateBasicMemory(512, initial_sp);

    reinterpret_cast<Instruction_t&>(memory[start_addr]) = Opcode::invalid;

    Processor processor{ memory };


    while (processor.state.registers[Processor_state::program_counter] < start_addr)
    {
        processor.executeNext();
    }

    EXPECT_EQ(processor.state.registers[Processor_state::program_counter], start_addr);
    EXPECT_EQ(processor.state.registers[Processor_state::stack_pointer], initial_sp);
}

TEST(interrupts, invalid_opcode)
{
    const auto invalid_opcode_handler_addr = 0xFF;
    auto [memory, start_addr] = generateBasicMemory(512, 512);

    reinterpret_cast<Instruction_t&>(memory[start_addr]) = Opcode::invalid;

    Processor processor{ memory };

    processor.interruptVector().invalid_opcode = invalid_opcode_handler_addr;


    while (processor.state.registers[Processor_state::program_counter] <= start_addr)
    {
        processor.executeNext();
    }

    EXPECT_EQ(processor.state.registers[Processor_state::program_counter], invalid_opcode_handler_addr);
}
