#pragma once
#include "processor_state.h"
#include "control_flow_instructions.h"
#include "address_range.h"
#include "common.h"
#include "peripherals/peripherals.h"
#include <optional>
#include <map>

#include <fstream>


namespace emulator
{
    using Read_handler = void (*)(Processor_state& state, uint64_t address, uint8_t* dst, uint8_t size);
    using Write_handler = void (*)(Processor_state& state, uint64_t address, uint8_t* src, uint8_t size);

    inline std::map<Address_range, Read_handler> read_handlers = {
        {
            {0xFFFF0000, 0xFFFF0008},
            [](Processor_state& state, uint64_t address, uint8_t* dst, uint8_t size) {//test handler
                for(int i=0;i<size;i++) *dst++ = 42;
            }
        },
        {
            timer0_address,
            [](Processor_state& state, auto... args) { state.peripherals.timer0.read(state, args...); }
        },
        {
            timer1_address,
            [](Processor_state& state, auto... args) { state.peripherals.timer1.read(state, args...); }
        }
    };

    inline std::map<Address_range, Write_handler> write_handlers = {
        {
            {0xFFFF0008, 0xFFFF0010},
            [](Processor_state& state, uint64_t address, uint8_t* src, uint8_t size) {//test handler
                std::ofstream ofs{"test_mmap.txt", std::ios::app};
                for (int i = 0; i < size; i++) ofs << *src++;
            }
        },
        {
            timer0_address,
            [](Processor_state& state, auto... args) { state.peripherals.timer0.write(state, args...); }
        },
        {
            timer1_address,
            [](Processor_state& state, auto... args) { state.peripherals.timer1.write(state, args...); }
        }
    };


    template<typename T>
    std::optional<T> readMemory(Processor_state& state, uint64_t address)
    {
        const Address_range range = { address, address + sizeof(T) };

        if (range.end <= state.memory.size())
        {
            return reinterpret_cast<Aliasable<T>&>(state.memory[address]);
        }
        else if (auto it = read_handlers.find(range); it != read_handlers.end())
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
        else if (auto it = write_handlers.find(range); it != write_handlers.end())
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