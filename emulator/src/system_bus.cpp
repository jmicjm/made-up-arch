#include "system_bus.h"


namespace emulator
{
    void System_bus::registerReadHandler(Address_range range, Read_handler handler)
    {
        read_handlers.insert({range, std::move(handler)});
    }

    void System_bus::registerWriteHandler(Address_range range, Write_handler handler)
    {
        write_handlers.insert({range, std::move(handler)});
    }

    void System_bus::update(Processor_state& state)
    {
        for(auto& p : peripherals)
        {
            p->update(state);
        }
    }

    void System_bus::addPeripheral(std::shared_ptr<Peripheral> peripheral, std::optional<Address_range> read_range, std::optional<Address_range> write_range)
    {
        if (read_range) registerReadHandler(*read_range, [=](Processor_state& state, auto... args) { peripheral->read(state, args...); });
        if (write_range) registerWriteHandler(*write_range, [=](Processor_state& state, auto... args) { peripheral->write(state, args...); });

        peripherals.push_back(std::move(peripheral));
    }
}
