#pragma once
#include "processor_state.h"
#include "instruction_decoder.h"
#include "interrupt_vector.h"


namespace emulator
{
    class Processor
    {
        Instruction_decoder decoder;

    public:
        Processor_state state;

        Processor(std::vector<uint8_t> memory);
        Processor(size_t memory_size);

        void reset();
        void executeNext();
    };
}
