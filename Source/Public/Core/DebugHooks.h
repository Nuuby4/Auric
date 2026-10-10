#pragma once

#include <string>
#include <SDK/Transform.h>

#include <map>

namespace Kyber
{
struct Color32
{
    uint8_t r;
    uint8_t g;
    uint8_t b;
    uint8_t a;

    constexpr Color32(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 255)
        : r(r)
        , g(g)
        , b(b)
        , a(a)
    {}
};

class DebugRenderer
{
public:
    static DebugRenderer* Get();
    static void Draw2dText(int x, int y, Color32 color, const std::string& text, float scale);
};
}