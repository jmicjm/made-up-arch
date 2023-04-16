#include "control_flow_instructions.h"


namespace emulator
{
    void branch(Processor_state& state, uint64_t target_addr, bool link)
    {
        if (link) state.registers[Processor_state::link_register] = state.registers[Processor_state::program_counter];

        state.registers[Processor_state::program_counter] = target_addr;
    }

    void branch(Processor_state& state, uint64_t target_addr, uint8_t condition, bool link)
    {
        const auto branchToTarget = [&]
        {
            branch(state, target_addr, link);
        };

        switch (condition)
        {
        case always:
            branchToTarget();
            break;
        case equal:
            if(state.pswFields().zero == 1) branchToTarget();
            break;
        case not_equal:
            if (state.pswFields().zero == 0) branchToTarget();
            break;
        case carry:
            if (state.pswFields().carry == 1) branchToTarget();
            break;
        case not_carry:
            if (state.pswFields().carry == 0) branchToTarget();
            break;
        case negative:
            if (state.pswFields().negative == 1) branchToTarget();
            break;
        case not_negative:
            if (state.pswFields().negative == 0) branchToTarget();
            break;
        case overflow:
            if (state.pswFields().overflow == 1) branchToTarget();
            break;
        case not_overflow:
            if (state.pswFields().overflow == 0) branchToTarget();
            break;
        case higher:
            if (state.pswFields().zero == 0 && state.pswFields().negative == state.pswFields().overflow) branchToTarget();
            break;
        case higher_or_equal:
            if (state.pswFields().negative == state.pswFields().overflow) branchToTarget();
            break;
        case less:
            if (state.pswFields().negative != state.pswFields().overflow) branchToTarget();
            break;
        case less_or_equal:
            if (state.pswFields().zero == 1 || state.pswFields().negative != state.pswFields().overflow) branchToTarget();
            break;
        case uhigher:
            if (state.pswFields().zero == 0 && state.pswFields().carry == 0) branchToTarget();
            break;
        case uless_or_equal:
            if (state.pswFields().zero == 1 || state.pswFields().carry == 1) branchToTarget();
            break;
        }
    }

    void branch(Processor_state& state, Instruction_t instrucion)
    {
        const auto instr = instruction_cast<Branch_instruction>(instrucion);
        const auto target_addr = state.registers[Processor_state::program_counter] + (static_cast<int64_t>(instr.offset) << 2);

        branch(state, target_addr, instr.condition, instr.link);
    }

    void branchAbsolute(Processor_state& state, Instruction_t instrucion)
    {
        const auto instr = instruction_cast<Branch_absolute_instruction>(instrucion);

        branch(state, state.registers[instr.dst], instr.condition, instr.link);
    }
}