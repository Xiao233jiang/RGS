#pragma once

#include <iostream>
#include <string>

#define PI 3.14159265359
#define EPSILON 1e-5f

namespace RGS 
{

struct Vec2 
{
    float X;
    float Y;

    constexpr Vec2()
        : X(0.0f), Y(0.0f) {}
    constexpr Vec2(float x, float y)
        : X(x), Y(y) {}
    operator std::string() const
    {
        return "(" + std::to_string(X) + ", " + std::to_string(Y) + ")";
    }
    friend std::ostream& operator<<(std::ostream& os, const Vec2& vec) 
    {
        return os << (std::string)vec;
    }
};

struct Vec3
{
    float X, Y, Z;

    // Learn constexpr: https://learn.microsoft.com/zh-cn/cpp/cpp/constexpr-cpp?view=msvc-170
    constexpr Vec3()
        : X(0.0f), Y(0.0f), Z(0.0f) {}
    constexpr Vec3(float x, float y, float z)
        : X(x), Y(y), Z(z) {}

    operator Vec2() const { return { X, Y }; }

    operator std::string() const
    {
        return "(" + std::to_string(X) + ", " + std::to_string(Y) + ", " + std::to_string(X) + ")";
    }
    friend std::ostream& operator<<(std::ostream& os, const Vec3& vec)
    {
        return os << (std::string) vec;
    }
};



unsigned char Float2UChar(const float f);
float UChar2Float(const unsigned char c);


}