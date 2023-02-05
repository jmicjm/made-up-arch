#pragma once
#include <cstdint>


namespace emulator
{
    namespace Opcode
    {
        enum Opcode : uint8_t
        {
            mov,
            ldr,
            str,
            add,
            sub,
            andl,
            orl,
            xorl,
            inv,
            invalid
        };
    }


    namespace Data_type
    {
        enum Data_type : uint8_t
        {
            byte, doublebyte, quadbyte, octobyte
        };
    }


    using Instruction_t [[gnu::may_alias]] = uint64_t;

    template<typename T>
    concept Instruction = sizeof(T) == sizeof(Instruction_t);

    template<Instruction dst_type, Instruction src_type>
    dst_type instruction_cast(src_type instruction)
    {
        return reinterpret_cast<dst_type&>(instruction);
    }

    auto toInstruction(Instruction auto instruction)
    {
        return instruction_cast<Instruction_t>(instruction);
    }
}