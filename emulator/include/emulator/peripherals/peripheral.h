#pragma once
#include "../processor_state.fwd.h"
#include <cstdint>

namespace emulator
{
    class Peripheral
    {
        public:
            virtual void update(Processor_state& state) {};
            virtual bool read(Processor_state& state, uint64_t offset, uint8_t* dst, uint8_t size) const { return false; };
            virtual bool write(Processor_state& state, uint64_t offset, const uint8_t* src, uint8_t size) { return false; };
    };
}
