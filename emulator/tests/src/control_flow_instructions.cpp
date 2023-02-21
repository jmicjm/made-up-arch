#include <gtest/gtest.h>
#include "instruction_tests_common.h"
#include "emulator/processor.h"
#include "emulator/instructions.h"
#include <vector>
#include <utility>
#include <cstdint>


using namespace emulator;

TEST(control_flow_instructions, branch_always)
{
    testBranch(
        Branch_condition::always,
        {
            { .carry = D, .overflow = D, .negative = D, .zero = D }
        },
        { 1, -1, 4, -4 }
    );
}

TEST(control_flow_instructions, branch_equal)
{
    testBranch(
        Branch_condition::equal,
        {
            { .carry = D, .overflow = D, .negative = D, .zero = S }
        },
        { 1, -1, 4, -4 }
    );
}

TEST(control_flow_instructions, branch_not_equal)
{
    testBranch(
        Branch_condition::not_equal,
        {
            { .carry = D, .overflow = D, .negative = D, .zero = C }
        },
        { 1, -1, 4, -4 }
    );
}

TEST(control_flow_instructions, branch_carry)
{
    testBranch(
        Branch_condition::carry,
        {
            { .carry = S, .overflow = D, .negative = D, .zero = D }
        },
        { 1, -1, 4, -4 }
    );
}

TEST(control_flow_instructions, branch_not_carry)
{
    testBranch(
        Branch_condition::not_carry,
        {
            { .carry = C, .overflow = D, .negative = D, .zero = D }
        },
        { 1, -1, 4, -4 }
    );
}

TEST(control_flow_instructions, branch_negative)
{
    testBranch(
        Branch_condition::negative,
        {
            { .carry = D, .overflow = D, .negative = S, .zero = D }
        },
        { 1, -1, 4, -4 }
    );
}

TEST(control_flow_instructions, branch_not_negative)
{
    testBranch(
        Branch_condition::not_negative,
        {
            { .carry = D, .overflow = D, .negative = C, .zero = D }
        },
        { 1, -1, 4, -4 }
    );
}

TEST(control_flow_instructions, branch_overflow)
{
    testBranch(
        Branch_condition::overflow,
        {
            { .carry = D, .overflow = S, .negative = D, .zero = D }
        },
        { 1, -1, 4, -4 }
    );
}

TEST(control_flow_instructions, branch_not_overflow)
{
    testBranch(
        Branch_condition::not_overflow,
        {
            { .carry = D, .overflow = C, .negative = D, .zero = D }
        },
        { 1, -1, 4, -4 }
    );
}

TEST(control_flow_instructions, branch_higher)
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

TEST(control_flow_instructions, branch_higher_or_equal)
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

TEST(control_flow_instructions, branch_less)
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

TEST(control_flow_instructions, branch_less_or_equal)
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

TEST(control_flow_instructions, branch_uhigher)
{
    testBranch(
        Branch_condition::uhigher,
        {
            { .carry = C, .overflow = D, .negative = D, .zero = C }
        },
        { 1, -1, 4, -4 }
    );
}

TEST(control_flow_instructions, branch_uless_or_equal)
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