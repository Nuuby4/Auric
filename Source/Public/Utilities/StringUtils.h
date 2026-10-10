// Copyright Nuuby. All Rights Reserved.

#pragma once
#include <cstdint>
#include <string>
#include <vector>

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

    template<typename... Args>
    static std::string Format(const std::string& format, Args... args)
    {
        int size_s = std::snprintf(nullptr, 0, format.c_str(), args...) + 1; // Extra space for '\0'
        if (size_s <= 0)
        {
            throw std::runtime_error("Error during formatting.");
        }

        auto size = static_cast<size_t>(size_s);
        std::unique_ptr<char[]> buf(new char[size]);
        std::snprintf(buf.get(), size, format.c_str(), args...);
        return std::string(buf.get(), buf.get() + size - 1); // We don't want the '\0' inside
    }
};
}

