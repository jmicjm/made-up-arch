#pragma once
#include "processor_state.h"
#include "control_flow_instructions.h"
#include "address_range.h"
#include "common.h"
#include "rw_handlers.h"
#include <optional>


namespace emulator
{
    template<typename T>
    std::optional<T> readMemory(Processor_state& state, uint64_t address)
    {
        const Address_range range = { address, address + sizeof(T) };

        if (range.end <= state.memory.size())
        {
            return reinterpret_cast<Aliasable<T>&>(state.memory[address]);
        }
        else if (auto it = Rw_handlers::read_handlers.find(range); it != Rw_handlers::read_handlers.end())
        {
           const auto& [handler_range, read_handler] = *it;

            if (handler_range.contains(range))
            {
                T buf{};
                read_handler(state, address, reinterpret_cast<uint8_t*>(&buf), sizeof(T));
                return buf;
            }
        }

        branch(state, state.interruptVector().invalid_address, true);
        return {};
    }

    template<typename T>
    void writeMemory(Processor_state& state, uint64_t address, T data)
    {
        const Address_range range = { address, address + sizeof(T) };

        if (range.end <= state.memory.size())
        {
            reinterpret_cast<Aliasable<T>&>(state.memory[address]) = data;
            return;
        }
        else if (auto it = Rw_handlers::write_handlers.find(range); it != Rw_handlers::write_handlers.end())
        {
            const auto& [handler_range, write_handler] = *it;

            if (handler_range.contains(range))
            {
                write_handler(state, address, reinterpret_cast<uint8_t*>(&data), sizeof(T));
                return;
            }
        }

        branch(state, state.interruptVector().invalid_address, true);
    }
}