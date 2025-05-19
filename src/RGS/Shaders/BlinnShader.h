#pragma once

#include "ShaderBase.h"

#include "RGS/Texture.h"
#include "RGS/Maths.h"
#include <ostream>

namespace RGS {

struct BlinnVertex : public VertexBase
{
    Vec3 ModelNormal;                            // 模型坐标系下法线
    Vec2 TexCoord = { 0.0f, 0.0f };         // 纹理坐标

    friend std::ostream& operator<<(std::ostream& os, const BlinnVertex& bv)
    {
        os << "BlinnVertex: { " << static_cast<const VertexBase&>(bv)  
            << ", ModelNormal: " << bv.ModelNormal
            << ", TexCoord: " << bv.TexCoord << " }";
        return os;
    }
};

struct BlinnVaryings : public VaryingsBase
{
    Vec3 WorldPos;          // 世界坐标位置
    Vec3 WorldNormal;       // 世界坐标下的法线
    Vec2 TexCoord;          // 纹理坐标
};

struct BlinnUniforms : public UniformsBase
{
    Mat4 Model;                                         // 模型变换矩阵
    Mat4 ModelNormalToWorld;                            // 模型法线变换到世界空间的矩阵
    Vec3 LightPos { 0.0f, 1.0f, 2.0f };         // 光源位置
    Vec3 LightAmbient { 0.3f, 0.3f, 0.3f };     // 环境光颜色
    Vec3 LightDiffuse { 0.5f, 0.5f, 0.5f };     // 漫反射光颜色
    Vec3 LightSpecular { 1.0f, 1.0f, 1.0f };    // 镜面反射光颜色
    Vec3 ObjectColor { 1.0f, 1.0f, 1.0f };      // 物体颜色
    Vec3 CameraPos;                                     // 相机位置
    float Shininess = 32.0f;                            // 物体的镜面指数

    Texture* Diffuse = nullptr;
    Texture* Specular = nullptr;
};

void BlinnVertexShader(BlinnVaryings& varyings, const BlinnVertex& vertex, const BlinnUniforms& uniforms);

/**
 * @brief Blinn片段着色器
*/
Vec4 BlinnFragmentShader(bool& discard, const BlinnVaryings& varyings, const BlinnUniforms& uniforms);

}