#include "processor_state.h"
#include "common.h"
#include <iostream>
#include <bitset>
#include <iomanip>
#include <utility>


auto emulator::Processor_state::pswFields() const -> const Status_word&
{
    return reinterpret_cast<const Status_word&>(status_word);
}

auto emulator::Processor_state::pswFields() -> Status_word&
{
    return const_cast<Status_word&>(std::as_const(*this).pswFields());
}

Interrupt_vector& emulator::Processor_state::interruptVector()
{
    return const_cast<Interrupt_vector&>(std::as_const(*this).interruptVector());
}

const Interrupt_vector& emulator::Processor_state::interruptVector() const
{
    return reinterpret_cast<const Interrupt_vector&>(memory[0]);
}

void emulator::Processor_state::print() const
{
    std::cout << "====PSW====\n";
    std::cout << "----ZNVC\n";
    std::cout << std::bitset<sizeof(status_word) * 8>{ status_word } << "\n";
    std::cout << "====REGISTERS====\n";
    for (auto i = 0; i < registers.size(); i++)
    {
        std::cout << "r" << std::setw(2) << std::left << std::setfill(' ') << std::dec <<  i << ": 0x"
            << std::setw(16) << std::right << std::setfill('0') << std::hex << registers[i] << "\n";
    }

    auto printTimer = [&](const Timer& timer, const std::string& name)
    {
        using namespace std::string_literals;

        std::cout << std::setw(10) << std::left << std::setfill(' ');
        std::cout << "T"s + name + "val: ";
        std::cout << "0x" << std::setw(16) << std::right << std::setfill('0') << std::hex << timer.state.value << "\n";
        std::cout << std::setw(10) << std::left << std::setfill(' ');
        std::cout << "T"s + name + "rstval: ";
        std::cout << "0x" << std::setw(16) << std::right << std::setfill('0') << std::hex << timer.state.reset_value << "\n";
        std::cout << std::setw(10) << std::left << std::setfill(' ');
        std::cout << "T"s + name + "conf: ";
        std::cout << "0x" << std::setw(16) << std::right << std::setfill('0') << std::hex << reinterpret_cast<Aliasable<const uint64_t>&>(timer.state.config_word) << "\n";
    };

    printTimer(peripherals.timer0, "0");
    printTimer(peripherals.timer1, "1");


    
    std::cout << "====MEMORY====\n";

    for (auto i = 0; i < memory.size(); i+=64)
    {
        for (auto j = 0; j < 64; j+=8)
        {
            std::cout << std::setw(16) << std::right << std::setfill('0') << std::hex << (uint64_t&)memory[i + j] << " ";
        }
        std::cout << "\n";
    }
}
