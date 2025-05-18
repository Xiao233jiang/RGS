#pragma once

#include "ShaderBase.h"

#include "RGS/Maths.h"

namespace RGS {

struct BlinnVertex : public VertexBase
{

};

struct BlinnVaryings : public VaryingsBase
{
};

struct BlinnUniforms : public UniformsBase
{
    bool IsAnother = false;
};

void BlinnVertexShader(BlinnVaryings& varyings, const BlinnVertex& vertex, const BlinnUniforms& uniforms);

/**
 * @brief Blinn片段着色器
*/
Vec4 BlinnFragmentShader(bool& discard, const BlinnVaryings& varyings, const BlinnUniforms& uniforms);

}