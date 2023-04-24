#pragma once
#include "processor_state.fwd.h"
#include "address_range.h"
#include "common.h"
#include <optional>
#include <map>


namespace emulator
{
    using Read_handler = void (*)(Processor_state& state, uint64_t address, uint8_t* dst, uint8_t size);
    using Write_handler = void (*)(Processor_state& state, uint64_t address, uint8_t* src, uint8_t size);

    class Rw_handlers
    {
        inline static std::map<Address_range, Read_handler> read_handlers;
        inline static std::map<Address_range, Write_handler> write_handlers;

        template<Address_range>
        friend class Register_read_handler;
        template<Address_range>
        friend class Register_write_handler;

        template<typename T>
        friend std::optional<T> readMemory(Processor_state& state, uint64_t address);

        template<typename T>
        friend void writeMemory(Processor_state& state, uint64_t address, T data);
    };

    template<Address_range>
    class Register_read_handler {};

    template<Address_range>
    class Register_write_handler {};
}

#define REGISTER_READ_HANDLER(address_range, handler)                       \
template<>                                                                  \
class Register_read_handler<address_range>                                  \
{                                                                           \
    struct Register                                                         \
    {                                                                       \
        Register()                                                          \
        {                                                                   \
            Rw_handlers::read_handlers.insert({ address_range, handler });  \
        }                                                                   \
    };                                                                      \
    inline static auto dummy = Register{};                                  \
}; 

#define REGISTER_WRITE_HANDLER(address_range, handler)                      \
template<>                                                                  \
class Register_write_handler<address_range>                                 \
{                                                                           \
    struct Register                                                         \
    {                                                                       \
        Register()                                                          \
        {                                                                   \
            Rw_handlers::write_handlers.insert({ address_range, handler }); \
        }                                                                   \
    };                                                                      \
    inline static auto dummy = Register{};                                  \
};      
