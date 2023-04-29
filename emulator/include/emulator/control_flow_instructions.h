#pragma once
#include "instruction.h"
#include "processor_state.h"
#include "instruction_registry.h"
#include "debugging/human_readable_instruction.h"


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

    const std::string_view branch_conditions[] = {
        "",
        "eq",
        "neq",
        "c",
        "nc",
        "neg",
        "nneg",
        "ov",
        "nov",
        "gt",
        "gtq",
        "le",
        "leq",
        "hi",
        "hiq"
    };

    struct [[gnu::may_alias]] Branch_instruction
    {
        uint32_t opcode : opcode_size = Opcode::branch;
        uint32_t link : 1;
        uint32_t condition : 4;
        int32_t offset : 19;
    };

    void branch(Processor_state& state, Instruction_t instrucion);
    REGISTER_INSTRUCTION(Opcode::branch, branch)
    REGISTER_INSTRUCTION_DESCRIBER(
        Opcode::branch,
        [](Instruction_t instr) {
            const auto i = instruction_cast<Branch_instruction>(instr);
            std::string desc = std::string{ "b" } + std::string{ branch_conditions[i.condition] };
            if (i.link) desc += "l";
            desc += " " + std::to_string(i.offset);

            return desc;
        }
    )

    struct [[gnu::may_alias]] Branch_absolute_instruction
    {
        uint32_t opcode : opcode_size = Opcode::branch_absolute;
        uint32_t link : 1;
        uint32_t condition : 4;
        uint32_t restore_from_interrupt : 1 = false;
        uint32_t dst : register_size;
    };

    void branchAbsolute(Processor_state& state, Instruction_t instrucion);
    REGISTER_INSTRUCTION(Opcode::branch_absolute, branchAbsolute)
    REGISTER_INSTRUCTION_DESCRIBER(
        Opcode::branch_absolute,
        [](Instruction_t instr) {
            const auto i = instruction_cast<Branch_absolute_instruction>(instr);

            if (i.restore_from_interrupt) return std::string{ "ir" };

            std::string desc = std::string{ "b" } + std::string{ branch_conditions[i.condition] };
            if (i.link) desc += "l";
            desc += " r" + std::to_string(i.dst);

            return desc;
        }
    )

    void branch(Processor_state& state, uint64_t target_addr, bool link);
    void branchInterrupt(Processor_state& state, uint64_t target_addr);
}