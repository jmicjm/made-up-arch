#pragma once
#include "processor_state.h"
#include "control_flow_instructions.h"
#include "common.h"
#include <optional>


namespace emulator
{
    template<typename T>
    std::optional<T> readMemory(Processor_state& state, uint64_t address)
    {
        if (address + sizeof(T) <= state.memory.size())
        {
            return reinterpret_cast<Aliasable<T>&>(state.memory[address]);
        }

        branch(state, state.interruptVector().invalid_address, true);
        return {};
    }

    template<typename T>
    void writeMemory(Processor_state& state, uint64_t address, T data)
    {
        if (address + sizeof(T) <= state.memory.size())
        {
            reinterpret_cast<Aliasable<T>&>(state.memory[address]) = data;
        }
        else branch(state, state.interruptVector().invalid_address, true);
    }
}