#include <gtest/gtest.h>
#include "emulator/processor.h"
#include "emulator/instructions.h"
#include <vector>
#include <utility>
#include <cstdint>
#include <string_view>
#include <algorithm>
#include <bit>


using namespace emulator;

TEST(simple_sequences_tests, load_update_store)
{
    const auto memory_val = 13;
    const auto addr = 0xFF;
    const auto register_val = 54;

    std::vector<Instruction_t> code;
    code.push_back(toInstruction(Mov_imm_instruction{ .rdst = 0, .imm = addr }));
    code.push_back(toInstruction(Ldr_instruction{ .size = Data_type::octobyte, .rdst = 1, .rbase = 0, .off = 0 }));
    code.push_back(toInstruction(Mov_imm_instruction{ .rdst = 2, .imm = register_val }));
    code.push_back(toInstruction(Arithmetic_instruction{ .opcode = Opcode::add, .rdst = 1, .r1 = 1, .r2 = 2 }));
    code.push_back(toInstruction(Str_instruction{ .size = Data_type::octobyte, .rsrc = 1, .rbase = 0, .off = 0 }));

    Processor processor{ 512 };

    for (auto i = 0; i < code.size(); i++)
    {
        reinterpret_cast<Instruction_t&>(processor.state.memory[i * sizeof(Instruction_t)]) = code[i];
    }
    reinterpret_cast<uint64_t&>(processor.state.memory[addr]) = memory_val;


    for (auto i = 0; i < code.size(); i++)
    {
        processor.executeNext();
    }

    EXPECT_EQ(processor.state.memory[addr], memory_val + register_val);
}

TEST(simple_sequences_tests, load_2lvl_indirect)
{
    const auto memory_val = 345;
    const auto addr1 = 0xFF;
    const auto addr2 = 0x1AA;
    const auto dst_register = 1;

    std::vector<Instruction_t> code;
    code.push_back(toInstruction(Mov_imm_instruction{ .rdst = 0, .imm = addr1 }));
    code.push_back(toInstruction(Ldr_instruction{ .size = Data_type::octobyte, .rdst = 0, .rbase = 0, .off = 0 }));
    code.push_back(toInstruction(Ldr_instruction{ .size = Data_type::octobyte, .rdst = dst_register, .rbase = 0, .off = 0 }));

    Processor processor{ 512 };

    for (auto i = 0; i < code.size(); i++)
    {
        reinterpret_cast<Instruction_t&>(processor.state.memory[i * sizeof(Instruction_t)]) = code[i];
    }
    reinterpret_cast<uint64_t&>(processor.state.memory[addr1]) = addr2;
    reinterpret_cast<uint64_t&>(processor.state.memory[addr2]) = memory_val;


    for (auto i = 0; i < code.size(); i++)
    {
        processor.executeNext();
    }

    EXPECT_EQ(processor.state.registers[dst_register], memory_val);
}

TEST(simple_sequences_tests, store_2lvl_indirect)
{
    const auto src_register = 0;
    const auto register_val = 345;
    const auto addr1 = 0xFF;
    const auto addr2 = 0x1AA;

    std::vector<Instruction_t> code;
    code.push_back(toInstruction(Mov_imm_instruction{ .rdst = 1, .imm = addr1 }));
    code.push_back(toInstruction(Ldr_instruction{ .size = Data_type::octobyte, .rdst = 1, .rbase = 1, .off = 0 }));
    code.push_back(toInstruction(Str_instruction{ .size = Data_type::octobyte, .rsrc = src_register, .rbase = 1, .off = 0 }));

    Processor processor{ 512 };

    processor.state.registers[src_register] = register_val;

    for (auto i = 0; i < code.size(); i++)
    {
        reinterpret_cast<Instruction_t&>(processor.state.memory[i * sizeof(Instruction_t)]) = code[i];
    }
    reinterpret_cast<uint64_t&>(processor.state.memory[addr1]) = addr2;


    for (auto i = 0; i < code.size(); i++)
    {
        processor.executeNext();
    }

    EXPECT_EQ(reinterpret_cast<uint64_t&>(processor.state.memory[addr2]), register_val);
}

TEST(simple_sequences_tests, fill_memory)
{
    const auto begin_addr = 0x100;
    const auto len = 16;
    const auto value = 0xFF;

    std::vector<Instruction_t> code;
    code.push_back(toInstruction(Mov_imm_instruction{ .rdst = 0, .imm = begin_addr }));
    code.push_back(toInstruction(Mov_imm_instruction{ .rdst = 1, .imm = len }));
    code.push_back(toInstruction(Mov_imm_instruction{ .rdst = 2, .imm = value }));
    code.push_back(toInstruction(Mov_imm_instruction{ .rdst = 3, .imm = 1 }));
    code.push_back(toInstruction(Arithmetic_instruction{ .opcode = Opcode::add, .rdst = 1, .r1 = 0, .r2 = 1 }));
    code.push_back(toInstruction(Comparison_instruction{ .opcode = Opcode::cmp, .r1 = 0, .r2 = 1 }));
    code.push_back(toInstruction(Branch_instruction{ .link = 0, .condition = Branch_condition::equal, .offset = 3 }));
    code.push_back(toInstruction(Str_instruction{ .size = Data_type::byte, .rsrc = 2, .rbase = 0, .off = 0 }));
    code.push_back(toInstruction(Arithmetic_instruction{ .opcode = Opcode::add, .rdst = 0, .r1 = 0, .r2 = 3 }));
    code.push_back(toInstruction(Branch_instruction{ .link = 0, .condition = Branch_condition::always, .offset = -5 }));

    Processor processor{ 512 };

    for (auto i = 0; i < code.size(); i++)
    {
        reinterpret_cast<Instruction_t&>(processor.state.memory[i * sizeof(Instruction_t)]) = code[i];
    }


    while (processor.state.registers[Processor_state::program_counter] < code.size() * sizeof(Instruction_t))
    {
        processor.executeNext();
    }

    EXPECT_EQ(processor.state.memory[begin_addr - 1], 0x0);
    for (auto i = 0; i < len; i++)
    {
        EXPECT_EQ(processor.state.memory[begin_addr + i], value);
    }
    EXPECT_EQ(processor.state.memory[begin_addr + len], 0x0);
}

TEST(simple_sequences_tests, strlen)
{
    const auto begin_addr = 0x100;
    const auto string = std::string_view{ "abcdefghij" };

    std::vector<Instruction_t> code;
    code.push_back(toInstruction(Mov_imm_instruction{ .rdst = 0, .imm = 0 }));
    code.push_back(toInstruction(Mov_imm_instruction{ .rdst = 1, .imm = begin_addr }));
    code.push_back(toInstruction(Ldr_instruction{ .size = Data_type::byte, .rdst = 2, .rbase = 1, .off = 0 }));
    code.push_back(toInstruction(Comparison_imm_instruction{ .opcode = Opcode::cmp, .r1 = 2, .imm = '\0'}));
    code.push_back(toInstruction(Branch_instruction{ .link = 0, .condition = Branch_condition::equal, .offset = 3 }));
    code.push_back(toInstruction(Arithmetic_imm_instruction{ .opcode = Opcode::add, .rdst = 0, .r1 = 0, .imm = 1 }));
    code.push_back(toInstruction(Arithmetic_imm_instruction{ .opcode = Opcode::add, .rdst = 1, .r1 = 1, .imm = 1 }));
    code.push_back(toInstruction(Branch_instruction{ .link = 0, .condition = Branch_condition::always, .offset = -6 }));

    Processor processor{ 512 };

    for (auto i = 0; i < code.size(); i++)
    {
        reinterpret_cast<Instruction_t&>(processor.state.memory[i * sizeof(Instruction_t)]) = code[i];
    }

    std::copy(string.begin(), string.end(), processor.state.memory.begin() + begin_addr);


    while (processor.state.registers[Processor_state::program_counter] < code.size() * sizeof(Instruction_t))
    {
        processor.executeNext();
    }

    EXPECT_EQ(processor.state.registers[0], string.size());
}

TEST(simple_sequences_tests, popcount)
{
    const auto val = 0xF000'0F00'00F0'000F;

    std::vector<Instruction_t> code;
    code.push_back(toInstruction(Mov_imm_instruction{ .rdst = 1, .imm = 0 }));
    code.push_back(toInstruction(Comparison_imm_instruction{ .opcode = Opcode::cmp, .r1 = 0, .imm = 0 }));
    code.push_back(toInstruction(Branch_instruction{ .link = 0, .condition = Branch_condition::equal, .offset = 4 }));
    code.push_back(toInstruction(Shift_imm_instruction{ .type = Shift_type::logical_left, .rdst = 0, .r1 = 0, .imm = 1 }));
    code.push_back(toInstruction(Branch_instruction{ .link = 0, .condition = Branch_condition::not_carry, .offset = 1 }));
    code.push_back(toInstruction(Arithmetic_imm_instruction{ .opcode = Opcode::add, .rdst = 1, .r1 = 1, .imm = 1 }));
    code.push_back(toInstruction(Branch_instruction{ .link = 0, .condition = Branch_condition::always, .offset = -6 }));

    Processor processor{ 512 };

    for (auto i = 0; i < code.size(); i++)
    {
        reinterpret_cast<Instruction_t&>(processor.state.memory[i * sizeof(Instruction_t)]) = code[i];
    }

    processor.state.registers[0] = val;


    while (processor.state.registers[Processor_state::program_counter] < code.size() * sizeof(Instruction_t))
    {
        processor.executeNext();
    }

    EXPECT_EQ(processor.state.registers[1], std::popcount(val));
}