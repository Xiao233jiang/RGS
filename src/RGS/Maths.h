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

struct Vec4
{
    float X, Y, Z, W;

    constexpr Vec4()
        : X(0.0f), Y(0.0f), Z(0.0f), W(0.0f) {}
    constexpr Vec4(float val)
        : X(val), Y(val), Z(val), W(val) {}
    constexpr Vec4(float x, float y, float z, float w)
        : X(x), Y(y), Z(z), W(w) {}
    constexpr Vec4(const Vec3& vec3, float w)
        : X(vec3.X), Y(vec3.Y), Z(vec3.Z), W(w) {}

    operator Vec2() const { return { X, Y }; }
    operator Vec3() const { return { X, Y, Z }; }

    operator std::string() const 
    {
        return "(" + std::to_string(X) + ", " + std::to_string(Y) + ", " + std::to_string(Z) + ", " + std::to_string(W) + ")";
    }
    friend std::ostream& operator<<(std::ostream& os, const Vec4& vec)
    {
        return os << (std::string)vec;
    }
};

// 按行优先存储，第一列向量为 [0][0] [0][1] [0][2] [0][3] <=> 0, 4, 8, 12
struct Mat4
{
    float M[4][4];

    Mat4()
    {
        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                M[i][j] = 0.0f;
            }
        }

    }

    Mat4(const Vec4& v0, const Vec4& v1, const Vec4& v2, const Vec4& v3);

    operator const std::string() const
    {
        std::string res;
        res += "(";

        for (int i = 0; i < 4; i++)
        {
            for (int j = 0; j < 4; j++)
            {
                res += std::to_string(M[i][j]);
                res += (i == 3 && j == 3) ? " )" : ", ";
            }
        }
        return res;
    }
};

Vec2 operator+ (const Vec2& left, const Vec2& right);
Vec2 operator- (const Vec2& left, const Vec2& right);

Vec3 operator+ (const Vec3& left, const Vec3& right);
Vec3 operator- (const Vec3& left, const Vec3& right);
Vec3 operator* (const float left, const Vec3& right);
Vec3 operator* (const Vec3& left, const float right);
Vec3 operator* (const Vec3& left, const Vec3& right);
Vec3 operator/ (const Vec3& left, const float right);

float Dot(const Vec3& left, const Vec3& right);     // 点乘
Vec3 Cross(const Vec3& left, const Vec3& right);    // 叉乘
Vec3 Normalize(const Vec3& v);      // 归一化， 长度为1

Vec4 operator+ (const Vec4& left, const Vec4& right);
Vec4 operator- (const Vec4& left, const Vec4& right);
Vec4 operator* (const float left, const Vec4& right);
Vec4 operator* (const Vec4& left, const float right);
Vec4 operator/ (const Vec4& left, const float right);
Vec4 operator* (const Mat4& mat4, const Vec4& vec4);
Mat4 operator* (const Mat4& left, const Mat4& right);
Mat4& operator*= (Mat4& left, const Mat4& right);

Mat4 Mat4Identity();    // 单位矩阵
Mat4 Mat4Translate(float tx, float ty, float tz);   // 平移矩阵
Mat4 Mat4Scale(float sx, float sy, float sz);       // 缩放矩阵
Mat4 Mat4RotateX(float angle);      // x轴旋转矩阵
Mat4 Mat4RotateY(float angle);      // y轴旋转矩阵
Mat4 Mat4RotateZ(float angle);      // z轴旋转矩阵
Mat4 Mat4LookAt(const Vec3& xAxis, const Vec3& yAxis, const Vec3& zAxis, const Vec3& eye);      // 视点矩阵
Mat4 Mat4LookAt(const Vec3& eye, const Vec3& target, const Vec3& up);       // 视点矩阵
Mat4 Mat4Perspective(float fovy, float aspect, float near, float far);      // 透视投影矩阵

// 线性插值， t 取值范围 [0, 1]
float Lerp(const float start, const float end, const float t);
Vec3 Lerp(const Vec3& start, const Vec3& end, const float t);
Vec4 Lerp(const Vec4& start, const Vec4& end, const float t);

unsigned char Float2UChar(const float f);   // 转换为0-255范围
float UChar2Float(const unsigned char c);   // 转换为0-1范围

float Clamp(const float in, const float min, const float max);  // 限制范围

}