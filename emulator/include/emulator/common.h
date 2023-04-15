#pragma once

namespace emulator
{
    auto lowerHalf(auto value)
    {
        const auto half_mask = (1ull << sizeof(value) * 8 / 2) - 1;
        return value & half_mask;
    }

    auto upperHalf(auto value)
    {
        const auto half_bits = sizeof(value) * 8 / 2;
        const auto half_mask = ((1ull << half_bits) - 1ull) << half_bits;
        return (value & half_mask) >> half_bits;
    }
}