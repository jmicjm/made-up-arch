#pragma once
#include "emulator/processor.h"
#include "emulator/instructions.h"
#include <vector>
#include <utility>
#include <cstdint>


using namespace emulator;

static void test2reg(const auto& instruction, const auto& expected, const std::vector<uint64_t>& operands)
{
    for (const auto& rsrc_val : operands)
    {
        for (auto rdst = 0u; rdst < (1 << register_size); rdst++)
        {
            for (auto rsrc = 0u; rsrc < (1 << register_size) - 1; rsrc++)
            {
                Processor processor{ sizeof(Instruction_t) };
                processor.state.registers[rsrc] = rsrc_val;
                processor.state.registers[Processor_state::program_counter] = 0x0;
                reinterpret_cast<Instruction_t&>(processor.state.memory[0x0]) = instruction(rdst, rsrc);
                const auto expected_val = expected(processor.state.registers[rsrc]);

                processor.executeNext();

                EXPECT_EQ(processor.state.registers[rdst], expected_val) << "rdst: " << rdst << " rsrc: " << rsrc;
            }
        }
    }
}

static void test3reg(const auto& instruction, const auto& expected, const std::vector<std::pair<uint64_t, uint64_t>>& operands)
{
    for (const auto [r1_val, r2_val] : operands)
    {
        for (auto rdst = 0u; rdst < (1 << register_size); rdst++)
        {
            for (auto r1 = 0u; r1 < (1 << register_size) - 1; r1++)
            {
                for (auto r2 = 0u; r2 < (1 << register_size) - 1; r2++)
                {
                    Processor processor{ sizeof(Instruction_t) };
                    processor.state.registers[r1] = r1_val;
                    processor.state.registers[r2] = r2_val;
                    processor.state.registers[Processor_state::program_counter] = 0x0;
                    reinterpret_cast<Instruction_t&>(processor.state.memory[0x0]) = instruction(rdst, r1, r2);
                    const auto expected_val = expected(processor.state.registers[r1], processor.state.registers[r2]);

                    processor.executeNext();

                    EXPECT_EQ(processor.state.registers[rdst], expected_val) << "rdst: " << rdst << " r1: " << r1 << " r2: " << r2;
                }
            }
        }
    }
}

enum Flag_value : uint8_t
{
    C = false,
    S = true,
    D
};

struct Flags
{
    Flag_value carry;
    Flag_value overflow;
    Flag_value negative;
    Flag_value zero;
};

static void test2regFlags(const auto& instruction, const std::vector<std::tuple<uint64_t, Flags>>& operands_and_expected)
{
    for (const auto& [rsrc, expected_flags] : operands_and_expected)
    {
        Processor processor{ sizeof(Instruction_t) };
        processor.state.registers[0] = rsrc;
        reinterpret_cast<Instruction_t&>(processor.state.memory[0x0]) = instruction(0u, 0u);

        processor.executeNext();

        if (expected_flags.carry != D) EXPECT_EQ(processor.state.pswFields().carry, expected_flags.carry);
        if (expected_flags.overflow != D) EXPECT_EQ(processor.state.pswFields().overflow, expected_flags.overflow);
        if (expected_flags.zero != D) EXPECT_EQ(processor.state.pswFields().zero, expected_flags.zero);
        if (expected_flags.negative != D) EXPECT_EQ(processor.state.pswFields().negative, expected_flags.negative);
    }
}

static void test2regComparisonFlags(const auto& instruction, const std::vector<std::tuple<uint64_t, uint64_t, Flags>>& operands_and_expected)
{
    for (const auto& [val1, val2, expected_flags] : operands_and_expected)
    {
        Processor processor{ sizeof(Instruction_t) };
        processor.state.registers[0] = val1;
        processor.state.registers[1] = val2;
        reinterpret_cast<Instruction_t&>(processor.state.memory[0x0]) = instruction(0u, 1u);

        processor.executeNext();

        if (expected_flags.carry != D) EXPECT_EQ(processor.state.pswFields().carry, expected_flags.carry);
        if (expected_flags.overflow != D) EXPECT_EQ(processor.state.pswFields().overflow, expected_flags.overflow);
        if (expected_flags.zero != D) EXPECT_EQ(processor.state.pswFields().zero, expected_flags.zero);
        if (expected_flags.negative != D) EXPECT_EQ(processor.state.pswFields().negative, expected_flags.negative);
    }
}

static void test3regFlags(const auto& instruction, const std::vector<std::tuple<uint64_t, uint64_t, Flags>>& operands_and_expected)
{
    for (const auto& [val1, val2, expected_flags] : operands_and_expected)
    {
        Processor processor{ sizeof(Instruction_t) };
        processor.state.registers[0] = val1;
        processor.state.registers[1] = val2;
        reinterpret_cast<Instruction_t&>(processor.state.memory[0x0]) = instruction(0u, 0u, 1u);

        processor.executeNext();

        if (expected_flags.carry != D) EXPECT_EQ(processor.state.pswFields().carry, expected_flags.carry);
        if (expected_flags.overflow != D) EXPECT_EQ(processor.state.pswFields().overflow, expected_flags.overflow);
        if (expected_flags.zero != D) EXPECT_EQ(processor.state.pswFields().zero, expected_flags.zero);
        if (expected_flags.negative != D) EXPECT_EQ(processor.state.pswFields().negative, expected_flags.negative);
    }
}

template<typename T>
static void ldrTest(uint8_t data_type, T memory_val, std::vector<std::pair<int32_t, int64_t>> base_offset_pairs)
{
    for (const auto [offset, base] : base_offset_pairs)
    {
        const auto data_addr = base + offset;
        const auto instruction_addr = data_addr + sizeof(T);
        const auto memory_size = instruction_addr + sizeof(Instruction_t);

        for (auto rdst = 0u; rdst < (1 << register_size); rdst++)
        {
            for (auto rbase = 0u; rbase < (1 << register_size) - 1; rbase++)
            {
                Processor processor{ memory_size };
                processor.state.registers[Processor_state::program_counter] = instruction_addr;
                processor.state.registers[rbase] = base;
                reinterpret_cast<Instruction_t&>(processor.state.memory[instruction_addr]) = toInstruction(Ldr_instruction{ .size = data_type, .rdst = rdst, .rbase = rbase, .off = offset });
                reinterpret_cast<T&>(processor.state.memory[data_addr]) = memory_val;

                processor.executeNext();

                EXPECT_EQ(processor.state.registers[rdst], memory_val) << "rdst: " << rdst << " rbase: " << rbase << " base: " << base << " offset: " << offset;
            }
        }
    }
}

template<typename T>
static void ldrTestPcRelative(uint8_t data_type, T memory_val, std::vector<int32_t> offsets)
{
    for (const auto offset : offsets)
    {
        const auto instruction_addr = offset >= 0 ? 0 : -offset - sizeof(Instruction_t);
        const auto data_addr = offset >= 0 ? instruction_addr + sizeof(Instruction_t) + offset : 0;
        const auto memory_size = offset >= 0 ? data_addr + sizeof(T) : instruction_addr + sizeof(Instruction_t);

        for (auto rdst = 0u; rdst < (1 << register_size); rdst++)
        {
            Processor processor{ memory_size };
            processor.state.registers[Processor_state::program_counter] = instruction_addr;
            reinterpret_cast<Instruction_t&>(processor.state.memory[instruction_addr]) = toInstruction(Ldr_instruction{ .size = data_type, .rdst = rdst, .rbase = Processor_state::program_counter, .off = offset });
            reinterpret_cast<T&>(processor.state.memory[data_addr]) = memory_val;

            processor.executeNext();

            EXPECT_EQ(processor.state.registers[rdst], memory_val) << "rdst: " << rdst << " offset: " << offset;
        }
    }
}

template<typename T>
static void strTest(uint8_t data_type, T register_val, std::vector<std::pair<int32_t, int64_t>> base_offset_pairs)
{
    for (const auto [offset, base] : base_offset_pairs)
    {
        const auto data_addr = base + offset;
        const auto instruction_addr = data_addr + sizeof(T);
        const auto memory_size = instruction_addr + sizeof(Instruction_t);

        for (auto rsrc = 0u; rsrc < (1 << register_size) - 1; rsrc++)
        {
            for (auto rbase = 0u; rbase < (1 << register_size) - 1; rbase++)
            {
                if (rbase == rsrc) continue;

                Processor processor{ memory_size };
                processor.state.registers[Processor_state::program_counter] = instruction_addr;
                processor.state.registers[rsrc] = register_val;
                processor.state.registers[rbase] = base;
                reinterpret_cast<Instruction_t&>(processor.state.memory[instruction_addr]) = toInstruction(Str_instruction{ .size = data_type, .rsrc = rsrc, .rbase = rbase, .off = offset });

                processor.executeNext();

                EXPECT_EQ(reinterpret_cast<T&>(processor.state.memory[data_addr]), register_val) << "rsrc: " << rsrc << " rbase: " << rbase << " base: " << base << " offset: " << offset;
            }
        }
    }
}

static void testBranch(Branch_condition condition, const std::vector<Flags>& flag_patterns, const std::vector<int32_t>& offsets)
{
    for (auto i = 0u; i < 16u; i++)
    {
        const Processor_state::Status_word psw = { .carry = i & 1, .overflow = (i & 2) >> 1, .negative = (i & 4) >> 2, .zero = (i & 8) >> 3 };

        const auto fulfilled = [&]
        {
            bool match = false;

            for (const auto flags : flag_patterns)
            {
                const auto flagValid = [](Flag_value flag_mask, bool flag_value)
                {
                    return flag_mask == D || flag_mask == flag_value;
                };
                match |= flagValid(flags.carry, psw.carry) && flagValid(flags.overflow, psw.overflow) && flagValid(flags.negative, psw.negative) && flagValid(flags.zero, psw.zero);
            }
            return match;
        }();

        for (const auto offset : offsets)
        {
            for (const auto link : std::vector{ 0u, 1u })
            {
                const auto instruction_addr = offset >= 0 ? 0 : -offset * 4 - sizeof(Instruction_t);
                const auto branch_addr = offset >= 0 ? instruction_addr + sizeof(Instruction_t) + offset * 4 : 0;
                const auto memory_size = offset >= 0 ? branch_addr + sizeof(Instruction_t) : instruction_addr + sizeof(Instruction_t);

                Processor processor{ memory_size };
                processor.state.registers[Processor_state::program_counter] = instruction_addr;
                processor.state.pswFields() = psw;
                reinterpret_cast<Instruction_t&>(processor.state.memory[instruction_addr]) = toInstruction(Branch_instruction{ .link = link, .condition = condition, .offset = offset });

                processor.executeNext();

                if (fulfilled) EXPECT_EQ(processor.state.registers[Processor_state::program_counter], branch_addr);
                else EXPECT_EQ(processor.state.registers[Processor_state::program_counter], instruction_addr + sizeof(Instruction_t));

                if(fulfilled && link) EXPECT_EQ(processor.state.registers[Processor_state::link_register], instruction_addr + sizeof(Instruction_t));
                else EXPECT_EQ(processor.state.registers[Processor_state::link_register], 0x0);
            }
        }
    }
}
