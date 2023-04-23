#pragma once
#include <cstdint>


namespace emulator
{
    struct Address_range
    {
        uint64_t begin = 0;
        uint64_t end = 0;

        bool operator<(const Address_range& other) const
        {
            return end <= other.begin;
        }

        bool contains(const Address_range& other) const
        {
            return other.begin >= begin && other.end <= end;
        }
    };
}