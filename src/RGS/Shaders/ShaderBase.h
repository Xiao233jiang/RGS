#pragma once

#include <string>
#include "RGS/Maths.h"

namespace RGS {

struct VertexBase
{
    Vec4 ModelPos = { 0, 0, 0, 1 };     // 模型空间坐标
    operator const std::string() const { return "ModelPos: " + (std::string)ModelPos; }     // 输出模型空间坐标
};

struct VaryingsBase
{
    Vec4 ClipPos = { 0, 0, 0, 1 };      // 裁剪空间坐标
};

struct UniformsBase
{
    Mat4 MVP;       // 变换矩阵
    operator const std::string() const { return (std::string)MVP; }     // 输出变换矩阵
};

}