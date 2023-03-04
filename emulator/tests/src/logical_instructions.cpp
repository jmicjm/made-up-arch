#include <gtest/gtest.h>
#include "instruction_tests_common.h"
#include "emulator/processor.h"
#include "emulator/instructions.h"
#include <vector>
#include <utility>
#include <cstdint>
#include <bit>


using namespace emulator;

TEST(logical_instructions, inv)
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

TEST(logical_instructions, inv_flags)
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

TEST(logical_instructions, andl)
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

TEST(logical_instructions, andl_flags)
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

TEST(logical_instructions, orl)
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

TEST(logical_instructions, orl_flags)
{
    test3regFlags(
        [](auto rdst, auto r1, auto r2) {
            return toInstruction(Logical_instruction{ .opcode = Opcode::orl, .rdst = rdst, .r1 = r1, .r2 = r2 });
        },
        {
            { 0x0000'0000'0000'0001, 0x0000'0000'0000'0001, { .carry = D, .overflow = D, .negative = C, .zero = C }},
            { 0x0000'0000'0000'0000, 0x0000'0000'0000'0000, { .carry = D, .overflow = D, .negative = C, .zero = S }},
            { 0x0000'0000'0000'0000, 0x8000'0000'0000'0000, { .carry = D, .overflow = D, .negative = S, .zero = C }},
        }
    );
}

TEST(logical_instructions, xorl)
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

TEST(logical_instructions, xorl_flags)
{
    test3regFlags(
        [](auto rdst, auto r1, auto r2) {
            return toInstruction(Logical_instruction{ .opcode = Opcode::xorl, .rdst = rdst, .r1 = r1, .r2 = r2 });
        },
        {
            { 0x0000'0000'0000'0000, 0x0000'0000'0000'0001, { .carry = D, .overflow = D, .negative = C, .zero = C }},
            { 0x0000'0000'0000'0001, 0x0000'0000'0000'0001, { .carry = D, .overflow = D, .negative = C, .zero = S }},
            { 0x0000'0000'0000'0000, 0x8000'0000'0000'0000, { .carry = D, .overflow = D, .negative = S, .zero = C }},
        }
    );
}

TEST(logical_instructions, shift_logical_left)
{
    test3reg(
        [](auto rdst, auto r1, auto r2) {
            return toInstruction(Shift_instruction{ .type = Shift_type::logical_left, .rdst = rdst, .r1 = r1, .r2 = r2 });
        },
        [](auto r1_val, auto r2_val) {
            return r1_val << (r2_val & (1ull << 6) - 1);
        },
        { 
            { 0x0000'0000'0000'0001, 0 },
            { 0x0000'0000'0000'0001, 1 },
            { 0xFFFF'FFFF'FFFF'FFFF, 32 },
            { 0x0000'0000'0000'0001, 63 },
            { 0x0000'0000'0000'0001, 64 },
            { 0x0000'0000'0000'0001, 65 }
        }
    );
}

TEST(logical_instructions, shift_logical_left_flags)
{
    test3regFlags(
        [](auto rdst, auto r1, auto r2) {
            return toInstruction(Shift_instruction{ .type = Shift_type::logical_left, .rdst = rdst, .r1 = r1, .r2 = r2 });
        },
        {
            { 0x0000'0000'0000'0001, 0,  { .carry = D, .overflow = D, .negative = C, .zero = C }},
            { 0x0000'0000'0000'0001, 1,  { .carry = C, .overflow = D, .negative = C, .zero = C }},
            { 0x8000'0000'0000'0001, 1,  { .carry = S, .overflow = D, .negative = C, .zero = C }},
            { 0x0000'0000'0000'0000, 1,  { .carry = C, .overflow = D, .negative = C, .zero = S }},
            { 0x8000'0000'0000'0000, 1,  { .carry = S, .overflow = D, .negative = C, .zero = S }},
            { 0x0000'0000'0000'0007, 61, { .carry = C, .overflow = D, .negative = S, .zero = C }},
            { 0x0000'0000'0000'0007, 62, { .carry = S, .overflow = D, .negative = S, .zero = C }},
        }
    );
}

TEST(logical_instructions, shift_arithmetic_left)
{
    test3reg(
        [](auto rdst, auto r1, auto r2) {
            return toInstruction(Shift_instruction{ .type = Shift_type::arithmetic_left, .rdst = rdst, .r1 = r1, .r2 = r2 });
        },
        [](auto r1_val, auto r2_val) {
            return r1_val << (r2_val & (1ull << 6) - 1);
        },
        {
            { 0x0000'0000'0000'0001, 0 },
            { 0x0000'0000'0000'0001, 1 },
            { 0xFFFF'FFFF'FFFF'FFFF, 32 },
            { 0x0000'0000'0000'0001, 63 },
            { 0x0000'0000'0000'0001, 64 },
            { 0x0000'0000'0000'0001, 65 }
        }
    );
}

TEST(logical_instructions, shift_arithmetic_left_flags)
{
    test3regFlags(
        [](auto rdst, auto r1, auto r2) {
            return toInstruction(Shift_instruction{ .type = Shift_type::arithmetic_left, .rdst = rdst, .r1 = r1, .r2 = r2 });
        },
        {
            { 0x0000'0000'0000'0001, 0,  { .carry = D, .overflow = D, .negative = C, .zero = C }},
            { 0x0000'0000'0000'0001, 1,  { .carry = C, .overflow = D, .negative = C, .zero = C }},
            { 0x8000'0000'0000'0001, 1,  { .carry = S, .overflow = D, .negative = C, .zero = C }},
            { 0x0000'0000'0000'0000, 1,  { .carry = C, .overflow = D, .negative = C, .zero = S }},
            { 0x8000'0000'0000'0000, 1,  { .carry = S, .overflow = D, .negative = C, .zero = S }},
            { 0x0000'0000'0000'0007, 61, { .carry = C, .overflow = D, .negative = S, .zero = C }},
            { 0x0000'0000'0000'0007, 62, { .carry = S, .overflow = D, .negative = S, .zero = C }},
        }
    );
}

TEST(logical_instructions, shift_logical_right)
{
    test3reg(
        [](auto rdst, auto r1, auto r2) {
            return toInstruction(Shift_instruction{ .type = Shift_type::logical_right, .rdst = rdst, .r1 = r1, .r2 = r2 });
        },
        [](auto r1_val, auto r2_val) {
            return r1_val >> (r2_val & (1ull << 6) - 1);
        },
        {
            { 0x0000'0000'0000'0001, 0 },
            { 0x0000'0000'0000'0001, 1 },
            { 0xFFFF'FFFF'FFFF'FFFF, 32 },
            { 0x0000'0000'0000'0001, 63 },
            { 0x0000'0000'0000'0001, 64 },
            { 0x0000'0000'0000'0001, 65 }
        }
    );
}

TEST(logical_instructions, shift_logical_right_flags)
{
    test3regFlags(
        [](auto rdst, auto r1, auto r2) {
            return toInstruction(Shift_instruction{ .type = Shift_type::logical_right, .rdst = rdst, .r1 = r1, .r2 = r2 });
        },
        {
            { 0x1000'0000'0000'0000, 0,  { .carry = D, .overflow = D, .negative = C, .zero = C }},
            { 0x8000'0000'0000'0000, 0,  { .carry = D, .overflow = D, .negative = S, .zero = C }},
            { 0x1000'0000'0000'0000, 1,  { .carry = C, .overflow = D, .negative = C, .zero = C }},
            { 0x0000'0000'0000'000F, 1,  { .carry = S, .overflow = D, .negative = C, .zero = C }},
            { 0x1000'0000'0000'0000, 62, { .carry = C, .overflow = D, .negative = C, .zero = S }},
            { 0x7000'0000'0000'0000, 63, { .carry = S, .overflow = D, .negative = C, .zero = S }}
        }
    );
}

TEST(logical_instructions, shift_arithmetic_right)
{
    test3reg(
        [](auto rdst, auto r1, auto r2) {
            return toInstruction(Shift_instruction{ .type = Shift_type::arithmetic_right, .rdst = rdst, .r1 = r1, .r2 = r2 });
        },
        [](auto r1_val, auto r2_val) {
            return static_cast<int64_t>(r1_val) >> (r2_val & (1ull << 6) - 1);
        },
        {
            { 0x0000'0000'0000'0001, 0 },
            { 0x0000'0000'0000'1000, 1 },
            { 0xFFFF'FFFF'FFFF'FFFF, 32 },
            { 0x8000'0000'0000'0000, 63 },
            { 0x8000'0000'0000'0000, 64 },
            { 0x8000'0000'0000'0000, 65 }
        }
    );
}

TEST(logical_instructions, shift_arithmetic_right_flags)
{
    test3regFlags(
        [](auto rdst, auto r1, auto r2) {
            return toInstruction(Shift_instruction{ .type = Shift_type::arithmetic_right, .rdst = rdst, .r1 = r1, .r2 = r2 });
        },
        {
            { 0x0000'0000'0000'0001, 0,  { .carry = D, .overflow = D, .negative = C, .zero = C }},
            { 0x0000'0000'0000'0010, 1,  { .carry = C, .overflow = D, .negative = C, .zero = C }},
            { 0x0000'0000'0000'0011, 1,  { .carry = S, .overflow = D, .negative = C, .zero = C }},
            { 0x0000'0000'0000'0001, 2,  { .carry = C, .overflow = D, .negative = C, .zero = S }},
            { 0x0000'0000'0000'0001, 1,  { .carry = S, .overflow = D, .negative = C, .zero = S }},
            { 0xF000'0000'0000'0000, 60, { .carry = C, .overflow = D, .negative = S, .zero = C }},
            { 0xF000'0000'0000'0000, 61, { .carry = S, .overflow = D, .negative = S, .zero = C }},
        }
    );
}

TEST(logical_instructions, rotate_left)
{
    test3reg(
        [](auto rdst, auto r1, auto r2) {
            return toInstruction(Rotate_instruction{ .type = Rotate_type::left, .rdst = rdst, .r1 = r1, .r2 = r2 });
        },
        [](auto r1_val, auto r2_val) {
            return std::rotl(r1_val, (r2_val & (1ull << 6) - 1));
        },
        {
            { 0x0000'0000'0000'0001, 0 },
            { 0x0000'0000'0000'0001, 1 },
            { 0xFFFF'FFFF'FFFF'FFFF, 32 },
            { 0x0000'0000'0000'0001, 63 },
            { 0x0000'0000'0000'0001, 64 },
            { 0x0000'0000'0000'0001, 65 }
        }
    );
}

TEST(logical_instructions, rotate_left_flags)
{
    test3regFlags(
        [](auto rdst, auto r1, auto r2) {
            return toInstruction(Rotate_instruction{ .type = Rotate_type::left, .rdst = rdst, .r1 = r1, .r2 = r2 });
        },
        {
            { 0x0000'0000'0000'0001, 0,  { .carry = D, .overflow = D, .negative = C, .zero = C }},
            { 0x0000'0000'0000'0001, 1,  { .carry = C, .overflow = D, .negative = C, .zero = C }},
            { 0x8000'0000'0000'0000, 1,  { .carry = S, .overflow = D, .negative = C, .zero = C }},
            { 0x0000'0000'0000'0000, 1,  { .carry = C, .overflow = D, .negative = C, .zero = S }},
            { 0x0000'0000'0000'0007, 61, { .carry = C, .overflow = D, .negative = S, .zero = C }},
            { 0x0000'0000'0000'0007, 62, { .carry = S, .overflow = D, .negative = S, .zero = C }},
        }
    );
}

TEST(logical_instructions, rotate_right)
{
    test3reg(
        [](auto rdst, auto r1, auto r2) {
            return toInstruction(Rotate_instruction{ .type = Rotate_type::right, .rdst = rdst, .r1 = r1, .r2 = r2 });
        },
        [](auto r1_val, auto r2_val) {
            return std::rotr(r1_val, (r2_val & (1ull << 6) - 1));
        },
        {
            { 0x0000'0000'0000'0001, 0 },
            { 0x0000'0000'0000'0001, 1 },
            { 0xFFFF'FFFF'FFFF'FFFF, 32 },
            { 0x0000'0000'0000'0001, 63 },
            { 0x0000'0000'0000'0001, 64 },
            { 0x0000'0000'0000'0001, 65 }
        }
    );
}

TEST(logical_instructions, rotate_right_flags)
{
    test3regFlags(
        [](auto rdst, auto r1, auto r2) {
            return toInstruction(Rotate_instruction{ .type = Rotate_type::right, .rdst = rdst, .r1 = r1, .r2 = r2 });
        },
        {
            { 0x0000'0000'0000'0000, 0,  { .carry = D, .overflow = D, .negative = C, .zero = S }},
            { 0x0000'0000'0000'0001, 0,  { .carry = D, .overflow = D, .negative = C, .zero = C }},
            { 0x0000'0000'0000'0001, 1,  { .carry = S, .overflow = D, .negative = S, .zero = C }},
            { 0x1000'0000'0000'0000, 1,  { .carry = C, .overflow = D, .negative = C, .zero = C }},
            { 0x7000'0000'0000'0000, 63, { .carry = S, .overflow = D, .negative = S, .zero = C }}
        }
    );
}