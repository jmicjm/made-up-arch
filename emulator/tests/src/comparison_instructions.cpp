#include <gtest/gtest.h>
#include "instruction_tests_common.h"
#include "emulator/processor.h"
#include "emulator/instructions.h"
#include <vector>
#include <utility>
#include <cstdint>


using namespace emulator;

TEST(comparison_instructions, tst_flags)
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

TEST(comparison_instructions, cmp_flags)
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