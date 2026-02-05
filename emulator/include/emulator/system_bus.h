#pragma once
#include "processor_state.fwd.h"
#include "address_range.h"
#include "peripherals/peripheral.h"
#include <optional>
#include <functional>
#include <map>
#include <memory>


namespace emulator
{
    using Read_handler = std::function<void(Processor_state& state, uint64_t offset, uint8_t* dst, uint8_t size)>;
    using Write_handler = std::function<void(Processor_state& state, uint64_t offset, const uint8_t* src, uint8_t size)>;

    class System_bus
    {
        std::vector<std::shared_ptr<Peripheral>> peripherals;
        std::map<Address_range, Read_handler> read_handlers;
        std::map<Address_range, Write_handler> write_handlers;
    public:

        template<typename T>
        std::optional<T> readMemory(Processor_state& state, uint64_t address);

        template<typename T>
        void writeMemory(Processor_state& state, uint64_t address, T data);

        void registerReadHandler(Address_range range, Read_handler handler);
        void registerWriteHandler(Address_range range, Write_handler handler);

        void update(Processor_state& state);
        void addPeripheral(std::shared_ptr<Peripheral> peripheral, std::optional<Address_range> read_range, std::optional<Address_range> write_range);
    };
}