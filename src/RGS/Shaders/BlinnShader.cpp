#include "BlinnShader.h"
#include "RGS/Maths.h"

#include <cmath>

namespace RGS {

void BlinnVertexShader(BlinnVaryings& varyings, const BlinnVertex& vertex, const BlinnUniforms& uniforms)
{
    varyings.ClipPos = uniforms.MVP * vertex.ModelPos;                                              // 计算顶点的裁剪空间位置     
    varyings.TexCoord = vertex.TexCoord;                                                            // 传递顶点的纹理坐标   
    varyings.WorldPos = uniforms.Model * vertex.ModelPos;                                           // 计算顶点的世界空间位置
    varyings.WorldNormal = uniforms.ModelNormalToWorld * Vec4{ vertex.ModelNormal, 0.0f };  // 计算顶点的世界空间法线，并将其转换为 Vec4 类型以便矩阵运算
}

Vec4 BlinnFragmentShader(bool& discard, const BlinnVaryings& varyings, const BlinnUniforms& uniforms)
{
    discard = false;

    // 获取相机位置、光源位置和世界位置
    const Vec3& cameraPos = uniforms.CameraPos;
    const Vec3& lightPos = uniforms.LightPos;
    const Vec3& worldPos = varyings.WorldPos;
    // 计算法线、视图方向和光源方向
    Vec3 worldNormal = Normalize(varyings.WorldNormal);
    Vec3 viewDir = Normalize(cameraPos - worldPos);
    Vec3 lightDir = Normalize(lightPos - worldPos);
    // 计算半角方向
    Vec3 halfDir = Normalize(lightDir + viewDir);

    // 获取环境光颜色
    Vec3 ambient = uniforms.LightAmbient;
    Vec3 specularStrength { 1.0f, 1.0f, 1.0f };
    Vec3 diffColor { 1.0f, 1.0f, 1.0f };
    if (uniforms.Diffuse && uniforms.Specular)
    {
        const Vec2& texCoord = varyings.TexCoord; 
        diffColor = uniforms.Diffuse->Sample(texCoord);
        ambient = ambient * diffColor;
        specularStrength = uniforms.Specular->Sample(texCoord);
    }
    // 计算漫反射光
    Vec3 diffuse = std::max(0.0f, Dot(worldNormal, lightDir)) * uniforms.LightDiffuse * diffColor;
    // 计算镜面反射光
    Vec3 specular = (float)pow(std::max(0.0f, Dot(halfDir, worldNormal)), uniforms.Shininess) * uniforms.LightSpecular * specularStrength;

    // 计算最终光照颜色
    Vec3 result = ambient + diffuse + specular;

    return { result, 1.0f };
}

} 