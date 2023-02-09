#include "processor_state.h"
#include <iostream>
#include <iomanip>


void emulator::Processor_state::print() const
{
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
