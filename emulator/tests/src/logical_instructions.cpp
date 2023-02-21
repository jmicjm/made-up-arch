#include <gtest/gtest.h>
#include "instruction_tests_common.h"
#include "emulator/processor.h"
#include "emulator/instructions.h"
#include <vector>
#include <utility>
#include <cstdint>


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