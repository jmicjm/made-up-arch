#pragma once
#include "instruction.h"
#include "processor_state.h"
#include "instruction_registry.h"


namespace emulator
{
    enum Branch_condition : uint8_t
    {
        always,
        equal,
        not_equal,
        carry, //uless
        not_carry, //uhigher_or_equal
        negative,
        not_negative,
        overflow,
        not_overflow,
        higher,
        higher_or_equal,
        less,
        less_or_equal,
        uhigher,
        uless_or_equal
    };

    struct [[gnu::may_alias]] Branch_instruction
    {
        uint32_t opcode : opcode_size = Opcode::branch;
        uint32_t link : 1;
        uint32_t condition : 4;
        int32_t offset : 19;
    };

    struct [[gnu::may_alias]] Branch_absolute_instruction
    {
        uint32_t opcode : opcode_size = Opcode::branch_absolute;
        uint32_t link : 1;
        uint32_t condition : 4;
        uint32_t dst : register_size;
    };

    void branch(Processor_state& state, Instruction_t instrucion);
    REGISTER_INSTRUCTION(Opcode::branch, branch);

    void branchAbsolute(Processor_state& state, Instruction_t instrucion);
    REGISTER_INSTRUCTION(Opcode::branch_absolute, branchAbsolute);

    void branch(Processor_state& state, uint64_t target_addr, bool link);
}