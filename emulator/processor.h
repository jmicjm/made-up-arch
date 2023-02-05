#pragma once
#include "processor_state.h"
#include "instruction_decoder.h"


namespace emulator
{
    class Processor
    {
        Instruction_decoder decoder;

    public:
        Processor_state state;

        Processor(size_t memory_size);
        void executeNext();
    };
}
