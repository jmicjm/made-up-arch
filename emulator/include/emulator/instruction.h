#pragma once
#include <cstdint>


namespace emulator
{
    constexpr auto opcode_size = 8;
    constexpr auto register_size = 4;
    constexpr auto data_type_size = 2;

    namespace Opcode
    {
        enum Opcode : uint8_t
        {
            mov,
            ldr,
            str,
            push,
            pop,
            add,
            sub,
            andl,
            orl,
            xorl,
            invl,
            shift,
            rotate,
            branch,
            branch_absolute,
            cmp,
            tst,
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


    using Instruction_t [[gnu::may_alias]] = uint32_t;

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