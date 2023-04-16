#include "memory_helpers.h"
#include <algorithm>
#include <cstring>


namespace emulator
{
    void writeCode(std::vector<uint8_t>& memory, uint64_t address, const std::vector<Instruction_t>& code)
    {
        const auto len = std::min(code.size() * sizeof(Instruction_t), memory.size() - address);

        std::memcpy(memory.data() + address, code.data(), len);
    }
}
