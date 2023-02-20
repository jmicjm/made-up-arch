#include <gtest/gtest.h>
#include "instruction_tests_common.h"
#include "emulator/processor.h"
#include "emulator/instructions.h"
#include <vector>
#include <utility>
#include <cstdint>


using namespace emulator;

TEST(instruction_tests, mov)
{
    test2reg(
        [](auto rdst, auto rsrc) {
            return toInstruction(Mov_instruction{ .rdst = rdst, .rsrc = rsrc });
        },
        [](auto rsrc_val) {
            return rsrc_val;
        },
        { 0xFFEEDDCCBBAA9988 }
    );
}

TEST(instruction_tests, mov_imm) 
{
    const auto imm = 0xFF;

    for (auto rdst = 0u; rdst < (1 << register_size); rdst++)
    {
        Processor processor{ sizeof(Instruction_t) };
        processor.state.registers[Processor_state::program_counter] = 0x0;
        reinterpret_cast<Instruction_t&>(processor.state.memory[0x0]) = toInstruction(Mov_imm_instruction{ .rdst = rdst, .imm = imm });

        processor.executeNext();

        EXPECT_EQ(processor.state.registers[rdst], imm) << "rdst: " << rdst << " imm: " << imm;
    }
}

TEST(instruction_tests, ldr_octobyte)
{
    ldrTest<uint64_t>(Data_type::octobyte, 0xFFEEDDCCBBAA9988, { {0,0}, {4,16}, {32,-8}, {-8,32} });
}

TEST(instruction_tests, ldr_quadbyte)
{
    ldrTest<uint32_t>(Data_type::quadbyte, 0xBBAA9988, { {0,0}, {4,16}, {32,-8}, {-8,32} });
}

TEST(instruction_tests, ldr_doublebyte)
{
    ldrTest<uint16_t>(Data_type::doublebyte, 0x9988, { {0,0}, {4,16}, {32,-8}, {-8,32} });
}

TEST(instruction_tests, ldr_byte)
{
    ldrTest<uint8_t>(Data_type::byte, 0x88, { {0,0}, {4,16}, {32,-8}, {-8,32} });
}
TEST(instruction_tests, ldr_octobyte_pc_relative)
{
    ldrTestPcRelative<uint64_t>(Data_type::octobyte, 0xFFEEDDCCBBAA9988, { -32, 0, 32 });
}

TEST(instruction_tests, ldr_quadbyte_pc_relative)
{
    ldrTestPcRelative<uint32_t>(Data_type::quadbyte, 0xBBAA9988, { -32, 0, 32 });
}

TEST(instruction_tests, ldr_doublebyte_pc_relative)
{
    ldrTestPcRelative<uint16_t>(Data_type::doublebyte, 0x9988, { -32, 0, 32 });
}

TEST(instruction_tests, ldr_byte_pc_relative)
{
    ldrTestPcRelative<uint8_t>(Data_type::byte, 0x88, { -32, 0, 32 });
}

TEST(instruction_tests, str_octobyte)
{
    strTest<uint64_t>(Data_type::octobyte, 0xFFEEDDCCBBAA9988, { {0,0}, {4,16}, {32,-8}, {-8,32} });
}

TEST(instruction_tests, str_quadbyte)
{
    strTest<uint32_t>(Data_type::quadbyte, 0xBBAA9988, { {0,0}, {4,16}, {32,-8}, {-8,32} });
}

TEST(instruction_tests, str_doublebyte)
{
    strTest<uint16_t>(Data_type::doublebyte, 0x9988, { {0,0}, {4,16}, {32,-8}, {-8,32} });
}

TEST(instruction_tests, str_byte)
{
    strTest<uint8_t>(Data_type::byte, 0x88, { {0,0}, {4,16}, {32,-8}, {-8,32} });
}

TEST(instruction_tests, inv)
{
    test2reg(
        [](auto rdst, auto rsrc) {
            return toInstruction(Logical_inv_instruction{ .rdst = rdst, .rsrc = rsrc });
        },
        [](auto rsrc_val) {
            return ~rsrc_val;
        },
        { 0xFFEEDDCCBBAA9988 }
    );
}

TEST(instruction_tests, inv_flags)
{
    test2regFlags(
        [](auto rdst, auto rsrc) {
            return toInstruction(Logical_inv_instruction{ .rdst = rdst, .rsrc = rsrc });
        },
        {
            { 0x8000'0000'0000'0000,  { .carry = D, .overflow = D, .negative = C, .zero = C } },
            { 0xFFFF'FFFF'FFFF'FFFF,  { .carry = D, .overflow = D, .negative = C, .zero = S } },
            { 0x0000'0000'0000'0000,  { .carry = D, .overflow = D, .negative = S, .zero = C } }
        }
    );
}

TEST(instruction_tests, add)
{
    test3reg(
        [](auto rdst, auto r1, auto r2) {
            return toInstruction(Arithmetic_instruction{ .opcode = Opcode::add, .rdst = rdst, .r1 = r1, .r2 = r2 });
        },
        [](auto r1_val, auto r2_val) {
            return r1_val + r2_val;
        },
        { {255, 65536} }
     );
}

TEST(instruction_tests, add_flags)
{
    test3regFlags(
        [](auto rdst, auto r1, auto r2) {
            return toInstruction(Arithmetic_instruction{ .opcode = Opcode::add, .rdst = rdst, .r1 = r1, .r2 = r2 });
        },
        {
            { 0x0000'0000'0000'0000, 0x0000'0000'0000'0001, { .carry = C, .overflow = C, .negative = C, .zero = C } },
            { 0x0000'0000'0000'0000, 0x0000'0000'0000'0000, { .carry = C, .overflow = C, .negative = C, .zero = S } },
            { 0x0000'0000'0000'0001, 0x8000'0000'0000'0000, { .carry = C, .overflow = C, .negative = S, .zero = C } },
            { 0x0000'0000'0000'0002, 0xFFFF'FFFF'FFFF'FFFF, { .carry = S, .overflow = C, .negative = C, .zero = C } },
            { 0x0000'0000'0000'0001, 0xFFFF'FFFF'FFFF'FFFF, { .carry = S, .overflow = C, .negative = C, .zero = S } },
            { 0xFFFF'FFFF'FFFF'FFFF, 0xFFFF'FFFF'FFFF'FFFF, { .carry = S, .overflow = C, .negative = S, .zero = C } },
            { 0x7FFF'FFFF'FFFF'FFFF, 0x7FFF'FFFF'FFFF'FFFF, { .carry = C, .overflow = S, .negative = S, .zero = C } },
            { 0x8000'0000'0000'0000, 0xFFFF'FFFF'FFFF'FFFF, { .carry = S, .overflow = S, .negative = C, .zero = C } },
            { 0x8000'0000'0000'0000, 0x8000'0000'0000'0000, { .carry = S, .overflow = S, .negative = C, .zero = S } }
        }
    );
}

TEST(instruction_tests, sub)
{
    test3reg(
        [](auto rdst, auto r1, auto r2) {
            return toInstruction(Arithmetic_instruction{ .opcode = Opcode::sub, .rdst = rdst, .r1 = r1, .r2 = r2 });
        },
        [](auto r1_val, auto r2_val) {
            return r1_val - r2_val;
        },
        { {255, 65536} }
    );
}

TEST(instruction_tests, sub_flags)
{
    test3regFlags(
        [](auto rdst, auto r1, auto r2) {
            return toInstruction(Arithmetic_instruction{ .opcode = Opcode::sub, .rdst = rdst, .r1 = r1, .r2 = r2 });
        },
        {
            { 0x0000'0000'0000'0002, 0x0000'0000'0000'0001, { .carry = C, .overflow = C, .negative = C, .zero = C } },
            { 0x0000'0000'0000'0001, 0x0000'0000'0000'0001, { .carry = C, .overflow = C, .negative = C, .zero = S } },
            { 0xFFFF'FFFF'FFFF'FFFF, 0x0000'0000'0000'0001, { .carry = C, .overflow = C, .negative = S, .zero = C } },
            { 0x0000'0000'0000'0000, 0x0000'0000'0000'0001, { .carry = S, .overflow = C, .negative = S, .zero = C } },
            { 0x8000'0000'0000'0000, 0x0000'0000'0000'0001, { .carry = C, .overflow = S, .negative = C, .zero = C } },
            { 0x0000'0000'0000'0000, 0x8000'0000'0000'0000, { .carry = S, .overflow = S, .negative = S, .zero = C } }
        }
    );
}

TEST(instruction_tests, andl)
{
    test3reg(
        [](auto rdst, auto r1, auto r2) {
            return toInstruction(Logical_instruction{ .opcode = Opcode::andl, .rdst = rdst, .r1 = r1, .r2 = r2 });
        },
        [](auto r1_val, auto r2_val) {
            return r1_val & r2_val;
        },
        { {0xF0F0F0F0F0F0F0F0, 0x0F0F0F0F0F0F0F0F}, {0xF0F0F0F0F0F0F0F0, 0xA1A1A1A1A1A1A1A1} }
    );
}

TEST(instruction_tests, andl_flags)
{
    test3regFlags(
        [](auto rdst, auto r1, auto r2) {
            return toInstruction(Logical_instruction{ .opcode = Opcode::andl, .rdst = rdst, .r1 = r1, .r2 = r2 });
        },
        {
            { 0x0000'0000'0000'0001, 0x0000'0000'0000'0001, { .carry = D, .overflow = D, .negative = C, .zero = C }},
            { 0x0000'0000'0000'0000, 0x0000'0000'0000'0001, { .carry = D, .overflow = D, .negative = C, .zero = S }},
            { 0x8000'0000'0000'0000, 0x8000'0000'0000'0000, { .carry = D, .overflow = D, .negative = S, .zero = C }},
        }
    );
}

TEST(instruction_tests, orl)
{
    test3reg(
        [](auto rdst, auto r1, auto r2) {
            return toInstruction(Logical_instruction{ .opcode = Opcode::orl, .rdst = rdst, .r1 = r1, .r2 = r2 });
        },
        [](auto r1_val, auto r2_val) {
            return r1_val | r2_val;
        },
        { {0xF0F0F0F0F0F0F0F0, 0x0F0F0F0F0F0F0F0F}, {0xF0F0F0F0F0F0F0F0, 0xA1A1A1A1A1A1A1A1} }
    );
}

TEST(instruction_tests, orl_flags)
{
    test3regFlags(
        [](auto rdst, auto r1, auto r2) {
            return toInstruction(Logical_instruction{ .opcode = Opcode::orl, .rdst = rdst, .r1 = r1, .r2 = r2 });
        },
        {
            { 0x0000'0000'0000'0001, 0x0000'0000'0000'0001, {.carry = D, .overflow = D, .negative = C, .zero = C }},
            { 0x0000'0000'0000'0000, 0x0000'0000'0000'0000, {.carry = D, .overflow = D, .negative = C, .zero = S }},
            { 0x0000'0000'0000'0000, 0x8000'0000'0000'0000, {.carry = D, .overflow = D, .negative = S, .zero = C }},
        }
    );
}

TEST(instruction_tests, xorl)
{
    test3reg(
        [](auto rdst, auto r1, auto r2) {
            return toInstruction(Logical_instruction{ .opcode = Opcode::xorl, .rdst = rdst, .r1 = r1, .r2 = r2 });
        },
        [](auto r1_val, auto r2_val) {
            return r1_val ^ r2_val;
        },
        { {0xF0F0F0F0F0F0F0F0, 0x0F0F0F0F0F0F0F0F}, {0xF0F0F0F0F0F0F0F0, 0xA1A1A1A1A1A1A1A1} }
    );
}

TEST(instruction_tests, xorl_flags)
{
    test3regFlags(
        [](auto rdst, auto r1, auto r2) {
            return toInstruction(Logical_instruction{ .opcode = Opcode::xorl, .rdst = rdst, .r1 = r1, .r2 = r2 });
        },
        {
            { 0x0000'0000'0000'0000, 0x0000'0000'0000'0001, {.carry = D, .overflow = D, .negative = C, .zero = C }},
            { 0x0000'0000'0000'0001, 0x0000'0000'0000'0001, {.carry = D, .overflow = D, .negative = C, .zero = S }},
            { 0x0000'0000'0000'0000, 0x8000'0000'0000'0000, {.carry = D, .overflow = D, .negative = S, .zero = C }},
        }
    );
}

TEST(instruction_tests, tst_flags)
{
    test2regComparisonFlags(
        [](auto r1, auto r2) {
            return toInstruction(Comparison_instruction{ .opcode = Opcode::tst, .r1 = r1, .r2 = r2 });
        },
        {
            { 0x0000'0000'0000'0001, 0x0000'0000'0000'0001, { .carry = D, .overflow = D, .negative = C, .zero = C }},
            { 0x0000'0000'0000'0000, 0x0000'0000'0000'0001, { .carry = D, .overflow = D, .negative = C, .zero = S }},
            { 0x8000'0000'0000'0000, 0x8000'0000'0000'0000, { .carry = D, .overflow = D, .negative = S, .zero = C }},
        }
    );
}

TEST(instruction_tests, cmp_flags)
{
    test2regComparisonFlags(
        [](auto r1, auto r2) {
            return toInstruction(Comparison_instruction{ .opcode = Opcode::cmp, .r1 = r1, .r2 = r2 });
        },
        {
            { 0x0000'0000'0000'0002, 0x0000'0000'0000'0001, { .carry = C, .overflow = C, .negative = C, .zero = C } },
            { 0x0000'0000'0000'0001, 0x0000'0000'0000'0001, { .carry = C, .overflow = C, .negative = C, .zero = S } },
            { 0xFFFF'FFFF'FFFF'FFFF, 0x0000'0000'0000'0001, { .carry = C, .overflow = C, .negative = S, .zero = C } },
            { 0x0000'0000'0000'0000, 0x0000'0000'0000'0001, { .carry = S, .overflow = C, .negative = S, .zero = C } },
            { 0x8000'0000'0000'0000, 0x0000'0000'0000'0001, { .carry = C, .overflow = S, .negative = C, .zero = C } },
            { 0x0000'0000'0000'0000, 0x8000'0000'0000'0000, { .carry = S, .overflow = S, .negative = S, .zero = C } }
        }
    );
}

TEST(instruction_tests, branch_always)
{
    testBranch(
        Branch_condition::always,
        {
            { .carry = D, .overflow = D, .negative = D, .zero = D } 
        },
        { 1, -1, 4, -4 }
    );
}

TEST(instruction_tests, branch_equal)
{
    testBranch(
        Branch_condition::equal,
        {
            { .carry = D, .overflow = D, .negative = D, .zero = S }
        },
        { 1, -1, 4, -4 }
    );
}
TEST(instruction_tests, branch_not_equal)
{
    testBranch(
        Branch_condition::not_equal,
        {
            { .carry = D, .overflow = D, .negative = D, .zero = C }
        },
        { 1, -1, 4, -4 }
    );
}

TEST(instruction_tests, branch_carry)
{
    testBranch(
        Branch_condition::carry,
        {
            { .carry = S, .overflow = D, .negative = D, .zero = D }
        },
        { 1, -1, 4, -4 }
    );
}

TEST(instruction_tests, branch_not_carry)
{
    testBranch(
        Branch_condition::not_carry,
        {
            { .carry = C, .overflow = D, .negative = D, .zero = D }
        },
        { 1, -1, 4, -4 }
    );
}

TEST(instruction_tests, branch_negative)
{
    testBranch(
        Branch_condition::negative,
        {
            { .carry = D, .overflow = D, .negative = S, .zero = D }
        },
        { 1, -1, 4, -4 }
    );
}

TEST(instruction_tests, branch_not_negative)
{
    testBranch(
        Branch_condition::not_negative,
        {
            { .carry = D, .overflow = D, .negative = C, .zero = D }
        },
        { 1, -1, 4, -4 }
    );
}

TEST(instruction_tests, branch_overflow)
{
    testBranch(
        Branch_condition::overflow,
        {
            { .carry = D, .overflow = S, .negative = D, .zero = D }
        },
        { 1, -1, 4, -4 }
    );
}

TEST(instruction_tests, branch_not_overflow)
{
    testBranch(
        Branch_condition::not_overflow,
        {
            { .carry = D, .overflow = C, .negative = D, .zero = D }
        },
        { 1, -1, 4, -4 }
    );
}

TEST(instruction_tests, branch_higher)
{
    testBranch(
        Branch_condition::higher,
        {
            { .carry = D, .overflow = C, .negative = C, .zero = C },
            { .carry = D, .overflow = S, .negative = S, .zero = C }
        },
        { 1, -1, 4, -4 }
    );
}

TEST(instruction_tests, branch_higher_or_equal)
{
    testBranch(
        Branch_condition::higher_or_equal,
        {
            { .carry = D, .overflow = C, .negative = C, .zero = D },
            { .carry = D, .overflow = S, .negative = S, .zero = D }
        },
        { 1, -1, 4, -4 }
    );
}

TEST(instruction_tests, branch_less)
{
    testBranch(
        Branch_condition::less,
        {
            { .carry = D, .overflow = C, .negative = S, .zero = D },
            { .carry = D, .overflow = S, .negative = C, .zero = D }
        },
        { 1, -1, 4, -4 }
    );
}

TEST(instruction_tests, branch_less_or_equal)
{
    testBranch(
        Branch_condition::less_or_equal,
        {
            { .carry = D, .overflow = D, .negative = D, .zero = S },
            { .carry = D, .overflow = C, .negative = S, .zero = D },
            { .carry = D, .overflow = S, .negative = C, .zero = D }
        },
        { 1, -1, 4, -4 }
    );
}

TEST(instruction_tests, branch_uhigher)
{
    testBranch(
        Branch_condition::uhigher,
        {
            { .carry = C, .overflow = D, .negative = D, .zero = C }
        },
        { 1, -1, 4, -4 }
    );
}

TEST(instruction_tests, branch_uless_or_equal)
{
    testBranch(
        Branch_condition::uless_or_equal,
        {
            { .carry = D, .overflow = D, .negative = D, .zero = S },
            { .carry = S, .overflow = D, .negative = D, .zero = D }
        },
        { 1, -1, 4, -4 }
    );
}