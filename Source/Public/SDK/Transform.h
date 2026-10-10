// Credit to Armchair Developers / Sean Kahler

#pragma once

#include <EASTL/string.h>
#include <EASTL/fixed_vector.h>

#include <glm/glm.hpp>

#include <string>

namespace Kyber
{
struct Vec2
{
    constexpr Vec2()
        : x(0.0f)
        , y(0.0f)
    {}

    constexpr Vec2(float _x, float _y)
        : x(_x)
        , y(_y)
    {}

    float x, y;
};

struct Vec3
{
    Vec3()
        : x(0)
        , y(0)
        , z(0)
    {}

    Vec3(float x, float y, float z)
        : x(x)
        , y(y)
        , z(z)
    {}

    Vec3 operator+(const Vec3& other) const
    { return Vec3(x + other.x, y + other.y, z + other.z); }

    Vec3 operator-(const Vec3& other) const
    { return Vec3(x - other.x, y - other.y, z - other.z); }

    Vec3 operator*(const Vec3& other) const
    { return Vec3(x * other.x, y * other.y, z * other.z); }

    Vec3 operator*(float scalar) const
    { return Vec3(x * scalar, y * scalar, z * scalar); }

    union
    {
        struct
        {
            float x, y, z;
            char _0x000C[4]; // 0x000C
        };

        __m128 simd;
    };

    std::string ToString() const
    { return "{" + std::to_string(x) + "," + std::to_string(y) + "," + std::to_string(z) + "}"; }

    glm::vec3 ToGlm() const
    { return glm::vec3(x, y, z); }
};

struct Vec4
{
    float x; // 0x0000
    float y; // 0x0004
    float z; // 0x0008
    float w; // 0x000C

    constexpr Vec4()
        : x(0.0f)
        , y(0.0f)
        , z(0.0f)
        , w(0.0f)
    {}

    Vec4(float x, float y, float z, float w)
        : x(x)
        , y(y)
        , z(z)
        , w(w)
    {}

    Vec4 operator*(float scalar) const
    { return Vec4(x * scalar, y * scalar, z * scalar, w * scalar); }

    Vec4 operator+(const Vec4& other) const
    { return Vec4(x + other.x, y + other.y, z + other.z, w + other.w); }

    Vec4 operator-(float scalar) const
    { return Vec4(x - scalar, y - scalar, z - scalar, w - scalar); }

    Vec4 operator-(const Vec4& other) const
    { return Vec4(x - other.x, y - other.y, z - other.z, w - other.w); }

    float Dot(const Vec4& other) const
    { return x * other.x + y * other.y + z * other.z + w * other.w; }

    Vec4 Normalize() const
    {
        float length = sqrtf(Dot(*this));
        return Vec4(x / length, y / length, z / length, w / length);
    }
};

struct LinearTransform
{
    Vec3 right;   // 0x0000
    Vec3 up;      // 0x0010
    Vec3 forward; // 0x0020
    Vec3 trans;   // 0x0030

    LinearTransform() = default;
    LinearTransform(const LinearTransform& other) = default;

    LinearTransform(float x, float y, float z)
        : right(1, 0, 0)
        , up(0, 1, 0)
        , forward(0, 0, 1)
        , trans(x, y, z)
    {}

    LinearTransform(
        float m1, float m2, float m3, float m4, float m5, float m6, float m7, float m8, float m9, float m10, float m11, float m12)
        : right(m1, m2, m3)
        , up(m4, m5, m6)
        , forward(m7, m8, m9)
        , trans(m10, m11, m12)
    {}

    LinearTransform(const Vec3& right, const Vec3& up, const Vec3& forward, const Vec3& trans)
        : right(right)
        , up(up)
        , forward(forward)
        , trans(trans)
    {}

    LinearTransform(LinearTransform orientation, Vec3 base)
        : right(orientation.right)
        , up(orientation.up)
        , forward(orientation.forward)
        , trans(base)
    {}

    std::string ToString() const
    { return "{" + right.ToString() + "," + up.ToString() + "," + forward.ToString() + "," + trans.ToString() + "}"; }

    glm::mat4 ToGlm() const
    { return glm::mat4(glm::vec4(right.ToGlm(), 0), glm::vec4(up.ToGlm(), 0), glm::vec4(forward.ToGlm(), 0), glm::vec4(trans.ToGlm(), 1)); }
};

template<int Mask>
inline __m128 vecShuffle(__m128 vec)
{ return _mm_shuffle_ps(vec, vec, Mask); }

inline __m128 vecMul(__m128 a, __m128 b)
{ return _mm_mul_ps(a, b); }

inline __m128 vecMulAdd(__m128 a, __m128 b, __m128 c)
{ return _mm_add_ps(_mm_mul_ps(a, b), c); }

inline LinearTransform TransformMultiply(const LinearTransform& a, const LinearTransform& b)
{
    // First set of shuffles (x component)
    __m128 sp0 = vecShuffle<_MM_SHUFFLE(0, 0, 0, 0)>(a.right.simd);
    __m128 sp1 = vecShuffle<_MM_SHUFFLE(0, 0, 0, 0)>(a.up.simd);
    __m128 sp2 = vecShuffle<_MM_SHUFFLE(0, 0, 0, 0)>(a.forward.simd);
    __m128 sp3 = vecShuffle<_MM_SHUFFLE(0, 0, 0, 0)>(a.trans.simd);

    __m128 bx = b.right.simd;
    __m128 ma0 = vecMul(sp0, bx);
    __m128 ma1 = vecMul(sp1, bx);
    __m128 ma2 = vecMul(sp2, bx);
    __m128 ma3 = vecMulAdd(sp3, bx, b.trans.simd);

    // Second set of shuffles (y component)
    sp0 = vecShuffle<_MM_SHUFFLE(1, 1, 1, 1)>(a.right.simd);
    sp1 = vecShuffle<_MM_SHUFFLE(1, 1, 1, 1)>(a.up.simd);
    sp2 = vecShuffle<_MM_SHUFFLE(1, 1, 1, 1)>(a.forward.simd);
    sp3 = vecShuffle<_MM_SHUFFLE(1, 1, 1, 1)>(a.trans.simd);

    __m128 by = b.up.simd;
    ma0 = vecMulAdd(sp0, by, ma0);
    ma1 = vecMulAdd(sp1, by, ma1);
    ma2 = vecMulAdd(sp2, by, ma2);
    ma3 = vecMulAdd(sp3, by, ma3);

    // Third set of shuffles (z component)
    sp0 = vecShuffle<_MM_SHUFFLE(2, 2, 2, 2)>(a.right.simd);
    sp1 = vecShuffle<_MM_SHUFFLE(2, 2, 2, 2)>(a.up.simd);
    sp2 = vecShuffle<_MM_SHUFFLE(2, 2, 2, 2)>(a.forward.simd);
    sp3 = vecShuffle<_MM_SHUFFLE(2, 2, 2, 2)>(a.trans.simd);

    __m128 bz = b.forward.simd;
    ma0 = vecMulAdd(sp0, bz, ma0);
    ma1 = vecMulAdd(sp1, bz, ma1);
    ma2 = vecMulAdd(sp2, bz, ma2);
    ma3 = vecMulAdd(sp3, bz, ma3);

    // Return the resulting transform
    LinearTransform result;
    result.right.simd = ma0;
    result.up.simd = ma1;
    result.forward.simd = ma2;
    result.trans.simd = ma3;

    return result;
}

inline LinearTransform& operator*=(LinearTransform& a, LinearTransform b)
{
    a = TransformMultiply(a, b);
    return a;
}

inline LinearTransform operator*(LinearTransform a, LinearTransform& b)
{ return TransformMultiply(a, b); }

inline LinearTransform linearTransformFromXRotation(float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);
    return LinearTransform(1, 0, 0, 0, c, s, 0, -s, c, 0, 0, 0);
}

inline LinearTransform linearTransformFromYRotation(float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);
    return LinearTransform(c, 0, -s, 0, 1, 0, s, 0, c, 0, 0, 0);
}

inline LinearTransform linearTransformFromZRotation(float angle)
{
    float c = cosf(angle);
    float s = sinf(angle);
    return LinearTransform(c, s, 0, -s, c, 0, 0, 0, 1, 0, 0, 0);
}
} // namespace Kyber
