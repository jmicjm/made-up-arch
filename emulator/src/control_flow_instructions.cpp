#include "control_flow_instructions.h"
#include "common.h"


namespace emulator
{
    void branch(Processor_state& state, uint64_t target_addr, bool link)
    {
        if (link) state.registers[Processor_state::link_register] = state.registers[Processor_state::program_counter];

        state.registers[Processor_state::program_counter] = target_addr;
    }

    void branchInterrupt(Processor_state& state, uint64_t target_addr)
    {
        if (state.registers[Processor_state::stack_pointer] < sizeof(uint8_t) + sizeof(uint64_t))
        {
            state.registers[Processor_state::program_counter] = state.interruptVector().handlers[Interrupts::reset];
            return;
        }

        state.registers[Processor_state::stack_pointer] -= sizeof(uint64_t);
        reinterpret_cast<Aliasable<uint64_t>&>(state.memory[state.registers[Processor_state::stack_pointer]]) = state.registers[Processor_state::program_counter];
        state.registers[Processor_state::stack_pointer] -= sizeof(uint8_t);
        reinterpret_cast<Aliasable<uint8_t>&>(state.memory[state.registers[Processor_state::stack_pointer]]) = state.status_word;

        state.registers[Processor_state::program_counter] = target_addr;

        state.pswFields() = Processor_state::Status_word{ .interrupt = 1 };
    }

    void restoreFromInterrupt(Processor_state& state)
    {
        if (state.registers[Processor_state::stack_pointer] + sizeof(uint8_t) + sizeof(uint64_t) > state.memory.size())
        {
            state.registers[Processor_state::program_counter] = state.interruptVector().handlers[Interrupts::reset];
            return;
        }

        state.status_word = reinterpret_cast<Aliasable<uint8_t>&>(state.memory[state.registers[Processor_state::stack_pointer]]);
        state.registers[Processor_state::stack_pointer] += sizeof(uint8_t);
        state.registers[Processor_state::program_counter] = reinterpret_cast<Aliasable<uint64_t>&>(state.memory[state.registers[Processor_state::stack_pointer]]);
        state.registers[Processor_state::stack_pointer] += sizeof(uint64_t);
    }

    bool branchConditionFulfilled(Processor_state& state, uint8_t condition)
    {
        switch (condition)
        {
        case always:
            return true;
        case equal:
            if (state.pswFields().zero == 1) return true;
            break;
        case not_equal:
            if (state.pswFields().zero == 0) return true;
            break;
        case carry:
            if (state.pswFields().carry == 1) return true;
            break;
        case not_carry:
            if (state.pswFields().carry == 0) return true;
            break;
        case negative:
            if (state.pswFields().negative == 1) return true;
            break;
        case not_negative:
            if (state.pswFields().negative == 0) return true;
            break;
        case overflow:
            if (state.pswFields().overflow == 1) return true;
            break;
        case not_overflow:
            if (state.pswFields().overflow == 0) return true;
            break;
        case higher:
            if (state.pswFields().zero == 0 && state.pswFields().negative == state.pswFields().overflow) return true;
            break;
        case higher_or_equal:
            if (state.pswFields().negative == state.pswFields().overflow) return true;
            break;
        case less:
            if (state.pswFields().negative != state.pswFields().overflow) return true;
            break;
        case less_or_equal:
            if (state.pswFields().zero == 1 || state.pswFields().negative != state.pswFields().overflow) return true;
            break;
        case uhigher:
            if (state.pswFields().zero == 0 && state.pswFields().carry == 0) return true;
            break;
        case uless_or_equal:
            if (state.pswFields().zero == 1 || state.pswFields().carry == 1) return true;
        }

        return false;
    }

    void branch(Processor_state& state, Instruction_t instrucion)
    {
        const auto instr = instruction_cast<Branch_instruction>(instrucion);
        const auto target_addr = state.registers[Processor_state::program_counter] + (static_cast<int64_t>(instr.offset) << 2);

        if (branchConditionFulfilled(state, instr.condition)) branch(state, target_addr, instr.link);
    }

    void branchAbsolute(Processor_state& state, Instruction_t instrucion)
    {
        const auto instr = instruction_cast<Branch_absolute_instruction>(instrucion);

        if (branchConditionFulfilled(state, instr.condition))
        {
            if (instr.restore_from_interrupt) restoreFromInterrupt(state);
            else branch(state, state.registers[instr.dst], instr.link);
        }
    }
}