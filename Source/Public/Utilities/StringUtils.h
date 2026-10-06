// Copyright Nuuby. All Rights Reserved.

#pragma once
#include <cstdint>

namespace Kyber
{
class StringUtils
{
public:
    static constexpr uint32_t HashQuick(const char* str)
    {
        uint32_t hash = 5381;

        char c = 0;
        while ((c = *str++))
        {
            hash = hash * 33 ^ uint32_t(c);
        }

        return hash;
    }
};
}

