#include "processor_state.h"
#include "processor_state.h"
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
