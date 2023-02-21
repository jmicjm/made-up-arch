#include <gtest/gtest.h>
#include "instruction_tests_common.h"
#include "emulator/processor.h"
#include "emulator/instructions.h"
#include <vector>
#include <utility>
#include <cstdint>


using namespace emulator;

TEST(arithmetic_instructions, add)
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

TEST(arithmetic_instructions, add_flags)
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

TEST(arithmetic_instructions, sub)
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

TEST(arithmetic_instructions, sub_flags)
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