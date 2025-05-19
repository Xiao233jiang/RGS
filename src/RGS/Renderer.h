#pragma once

#include "RGS/Framebuffer.h"
#include "RGS/Base.h"
#include "RGS/Maths.h"
#include "Shaders/ShaderBase.h"

#include <cstddef>
#include <cstdint>
#include <type_traits>
#include <cmath>


namespace RGS {

template<typename vertex_t>
struct Triangle 
{
    static_assert(std::is_base_of_v<VertexBase, vertex_t>, "vertex_t 必须继承自 RGS::VertexBase");
    
    vertex_t Vertex[3];

    // 重载 [] 操作符
    vertex_t& operator[](size_t i) { return Vertex[i]; }    
    const vertex_t& operator[](size_t i) const { return Vertex[i]; } 

    Triangle() = default;   // 默认构造函数, default指定了编译器生成默认构造函数

    friend std::ostream& operator<<(std::ostream& os, const Triangle<vertex_t>& triangle) 
    {
        os << "Triangle: {\n";
        for (int i = 0; i < 3; ++i) {
            os << "  Vertex[" << i << "]: " << triangle.Vertex[i] << "\n";
        }
        os << "}";
        return os;
    }
};

enum class DepthFuncType 
{
    LESS,           // 小于
    LEQUAL,         // 小于等于
    ALWAYS,         // 总是
};

template<typename vertex_t, typename uniforms_t, typename varyings_t>
struct Program 
{
    bool EnableDepthTest = true;      // 是否启用深度测试
    bool EnableWriteDepth = true;     // 是否启用深度写入
    bool EnableBlend = false;          // 是否启用混合
    bool EnableDoubleSided = false;   // 是否启用双面渲染

    DepthFuncType DepFunc = DepthFuncType::LESS;        // 深度测试函数类型

    using vertex_shader_t = void (*)(varyings_t&, const vertex_t&, const uniforms_t&);
    vertex_shader_t VertexShader;   // 顶点着色器

    // discard 为true表示当前判断片段被丢弃 
    using fragment_shader_t = Vec4(*)(bool& discard, const varyings_t&, const uniforms_t&);
    fragment_shader_t FragmentShader;   // 片段着色器

    Program(const vertex_shader_t vertexShader, const fragment_shader_t fragmentShader)
        : VertexShader(vertexShader),
        FragmentShader(fragmentShader)
    {}
};


class Renderer 
{
private:
    static constexpr int RGS_MAX_VARYINGS = 9;      // 最大插值变量数目

private:
    enum class Plane        
    {
        // 定义正方向和负方向的常量
        // POSITIVE 表示正方向
        POSITIVE_W,
        POSITIVE_X,
        NEGATIVE_X,
        POSITIVE_Y,
        NEGATIVE_Y,
        POSITIVE_Z,
        NEGATIVE_Z,
    };

    struct BoundingBox { int MinX, MaxX, MinY, MaxY; };   // 视锥体

    /**
     * @brief 判断点是否在视锥体内
     * @param clipPos 裁剪空间坐标
    */
    static bool IsVertexVisible(const Vec4& clipPos);
    /**
     * @brief 判断点是否在平面内
     * @param clipPos 裁剪空间坐标
     * @param plane 平面
    */
    static bool IsInsidePlane(const Vec4& clipPos, const Plane plane);
    /**
     * @brief 判断点是否在三角形内
     * @param weights 三角形比例
    */
    static bool IsInsideTriangle(float (&weights)[3]);
    /**
     * @brief 判断是否是背面
    */
    static bool IsBackFacing(const Vec4& a, const Vec4& b, const Vec4& c);
    /**
     * @brief 判断点是否在平面内
    */
    static bool PassDepthTest(const float writeDepth, const float fDepth, const DepthFuncType depthFunc);

    /**
     * @brief 计算线段与平面的交点比例
     * @param prev 前一点
     * @param curr 当前点
     * @param plane 平面
    */
    static float GetIntersectRatio(const Vec4& prev, const Vec4& curr, const Plane plane);
    /**
     * @brief 计算裁剪空间坐标
     * @param fragCoords 片段坐标
     * @param width 屏幕宽度
     * @param height 屏幕高度
    */
    static BoundingBox GetBoundingBox(const Vec4(&fragCoords)[3], const int width, const int height);
    /**
     * @brief 
    */
    static void CalculateWeights(float (&screenWeights)[3], float(&weights)[3], const Vec4(&fragCoords)[3], const Vec2& screenPoint);

    /**
     * @brief 计算平面方程的交点
     * @param out 输出交点
     * @param start 起点
     * @param end 终点
     * @param ratio 线段比例
    */
    template <typename varyings_t>
    static void LerpVaryings(varyings_t& out, const varyings_t& start, const varyings_t& end, const float ratio)
    {
        constexpr uint32_t floatNum = sizeof(varyings_t) / sizeof(float);   // 计算varyings_t结构体中float类型的数量
        float* startFloat = (float*)&start;     // 将start指针转换为指向float的指针
        float* endFloat = (float*)&end;         // 将end指针转换为指向float的指针
        float* outFloat = (float*)&out;         // 将out指针转换为指向float的指针

        for (int i = 0; i < (int)floatNum; i++)
        {
            outFloat[i] = Lerp(startFloat[i], endFloat[i], ratio);
        }
    }
    /**
     * @brief 计算三角形的插值变量
    */
    template <typename varyings_t>
    static void LerpVaryings(varyings_t& out, 
                                const varyings_t(&varyings)[3], 
                                const float(&weights)[3], 
                                const int width, 
                                const int height)
    {
        out.ClipPos = varyings[0].ClipPos * weights[0] + 
                        varyings[1].ClipPos * weights[1] + 
                        varyings[2].ClipPos * weights[2];
        out.NdcPos = out.ClipPos / out.ClipPos.W;
        out.NdcPos.W = 1.0f / out.ClipPos.W;
        out.FragPos.X = ((out.NdcPos.X + 1.0f) * 0.5f * 600);
        out.FragPos.Y = ((out.NdcPos.Y + 1.0f) * 0.5f * 300);
        out.FragPos.Z = (out.NdcPos.Z + 1.0f) * 0.5f;
        out.FragPos.W = out.NdcPos.W;

        constexpr uint32_t floatOffset = sizeof(Vec4) * 3 / sizeof(float);
        constexpr uint32_t floatNum = sizeof(varyings_t) / sizeof(float);
        float* v0 = (float*)&varyings[0];
        float* v1 = (float*)&varyings[1];
        float* v2 = (float*)&varyings[2];
        float* outFloat = (float*)&out;

        for (int i = floatOffset; i < (int)floatNum; i++)
        {
            outFloat[i] = v0[i] * weights[0] + v1[i] * weights[1] + v2[i] * weights[2];
        }
    }

    /**
     * @brief 裁剪三角形
     * @param outVaryings 输出插值变量
     * @param inVaryings 输入插值变量
     * @param plane 裁剪平面
     * @param inVertexNum 输入顶点数目
    */
    template <typename varyings_t>
    static int ClipAgainstPlane(varyings_t(&outVaryings)[RGS_MAX_VARYINGS],
                                const varyings_t(&inVaryings)[RGS_MAX_VARYINGS],
                                const Plane plane,
                                const int inVertexNum)
    {
        ASSERT(inVertexNum >= 3);

        int outVertexNum = 0;

        for (int i = 0; i < inVertexNum; i++)
        {
            int prevIndex = (inVertexNum - 1 + i) % inVertexNum;
            int currIndex = i;

            const varyings_t& prevVaryings = inVaryings[prevIndex];
            const varyings_t& currVaryings = inVaryings[currIndex];

            const bool prevInside = IsInsidePlane(prevVaryings.ClipPos, plane);
            const bool currInside = IsInsidePlane(currVaryings.ClipPos, plane);

            if (currInside != prevInside)
            {
                float ratio = GetIntersectRatio(prevVaryings.ClipPos, currVaryings.ClipPos, plane);
                LerpVaryings(outVaryings[outVertexNum], prevVaryings, currVaryings, ratio);
                outVertexNum++;
            }

            if (currInside)
            {
                outVaryings[outVertexNum] = inVaryings[currIndex];
                outVertexNum++;
            }
        }

        ASSERT(outVertexNum <= RGS_MAX_VARYINGS);
        return outVertexNum;
    }

    template<typename varyings_t>
    static int Clip(varyings_t(&varyings)[RGS_MAX_VARYINGS])
    {
        bool v0_Visble = IsVertexVisible(varyings[0].ClipPos);
        bool v1_Visble = IsVertexVisible(varyings[1].ClipPos);
        bool v2_Visble = IsVertexVisible(varyings[2].ClipPos);
        if (v0_Visble && v1_Visble && v2_Visble)
            return 3;

        int vertexNum = 3;
        varyings_t varyings_[RGS_MAX_VARYINGS];
        vertexNum = ClipAgainstPlane(varyings_, varyings, Plane::POSITIVE_W, vertexNum);
        if (vertexNum == 0) return 0;
        vertexNum = ClipAgainstPlane(varyings, varyings_, Plane::POSITIVE_X, vertexNum);
        if (vertexNum == 0) return 0;
        vertexNum = ClipAgainstPlane(varyings_, varyings, Plane::NEGATIVE_X, vertexNum);
        if (vertexNum == 0) return 0;
        vertexNum = ClipAgainstPlane(varyings, varyings_, Plane::POSITIVE_Y, vertexNum);
        if (vertexNum == 0) return 0;
        vertexNum = ClipAgainstPlane(varyings_, varyings, Plane::NEGATIVE_Y, vertexNum);
        if (vertexNum == 0) return 0;
        vertexNum = ClipAgainstPlane(varyings, varyings_, Plane::POSITIVE_Z, vertexNum);
        if (vertexNum == 0) return 0;
        vertexNum = ClipAgainstPlane(varyings_, varyings, Plane::NEGATIVE_Z, vertexNum);
        if (vertexNum == 0) return 0;
        memcpy(varyings, varyings_, sizeof(varyings_));

        return vertexNum;
    }

    /**
     * @brief 计算NDC坐标
     * @param varyings 输入插值变量
     * @param vertexNum 输入顶点数目
    */
    template<typename varyings_t>
    static void CaculateNdcPos(varyings_t(&varyings)[RGS_MAX_VARYINGS], const int vertexNum)
    {
        for (int i = 0; i < vertexNum; i++)
        {
            float w = varyings[i].ClipPos.W;
            varyings[i].NdcPos = varyings[i].ClipPos / w;
            varyings[i].NdcPos.W = 1.0f / w;
        }
    }
    /**
     * @brief 计算片段位置
     * @param varyings 输入插值变量
     * @param vertexNum 输入顶点数目
     * @param width 屏幕宽度
     * @param height 屏幕高度
    */
    template<typename varyings_t>
    static void CaculateFragPos(varyings_t(&varyings)[RGS_MAX_VARYINGS],
                                const int vertexNum,
                                const float width,
                                const float height)
    {
        for (int i = 0; i < vertexNum; i++)
        {
            // 将NDC坐标转换为屏幕坐标
            float x = ((varyings[i].NdcPos.X + 1.0f) * 0.5f * width);       
            float y = ((varyings[i].NdcPos.Y + 1.0f) * 0.5f * height);
            float z = (varyings[i].NdcPos.Z + 1.0f) * 0.5f;
            float w = varyings[i].NdcPos.W;

            varyings[i].FragPos.X = x;
            varyings[i].FragPos.Y = y;
            varyings[i].FragPos.Z = z;
            varyings[i].FragPos.W = w;
        }
    }

    template <typename vertex_t, typename uniforms_t, typename varyings_t>
    static void ProcessPixel(Framebuffer& framebuffer,
                                const int x,
                                const int y,
                                const Program<vertex_t, uniforms_t, varyings_t>& program,
                                const varyings_t& varyings,
                                const uniforms_t& uniforms)
    {
        /* Pixel Shading */
        bool discard = false;
        Vec4 color{ 0.0f, 0.0f, 0.0f, 0.0f };
        color = program.FragmentShader(discard, varyings, uniforms);
        if (discard)
        {
            return;
        }
        color.X = Clamp(color.X, 0.0f, 1.0f);
        color.Y = Clamp(color.Y, 0.0f, 1.0f);
        color.Z = Clamp(color.Z, 0.0f, 1.0f);
        color.W = Clamp(color.W, 0.0f, 1.0f);

        /* Blend (混合) */ /* 用于透明物体 */
        if (program.EnableBlend)    // 如果启用混合
        {
            Vec3 dstColor = framebuffer.GetColor(x, y);     // 读取当前像素颜色
            Vec3 srcColor = color;      // 读取片段颜色
            float alpha = color.W;      // 读取片段透明度
            color = { Lerp(dstColor, srcColor, alpha), 1.0f };  // 计算混合颜色
            framebuffer.SetColor(x, y, color);
        }
        else 
        {
            framebuffer.SetColor(x, y, color);
        } 

        if (program.EnableWriteDepth)   // 如果启用深度写入
        {
            float depth = varyings.FragPos.Z;
            framebuffer.SetDepth(x, y, depth);
        }
    }

    /**
     * @brief 绘制三角形
     * @param framebuffer 帧缓存
     * @param program 着色器程序
     * @param varyings 输入插值变量
     * @param uniforms 统一变量
    */
    template<typename vertex_t, typename uniforms_t, typename varyings_t>
    static void RasterizeTriangle(Framebuffer& framebuffer,
                                const Program<vertex_t, uniforms_t, varyings_t>& program,
                                const varyings_t(&varyings)[3],
                                const uniforms_t& uniforms)
    {
        /* Back Face Culling(背向剔除) */
        if (!program.EnableDoubleSided)     // 如果没有开启双面渲染
        {
            bool isBackFacing = false;
            isBackFacing = IsBackFacing(varyings[0].NdcPos, varyings[1].NdcPos, varyings[2].NdcPos);
            if (isBackFacing)
            {
                return;
            }
        }

        int width = framebuffer.GetWidth();
        int height = framebuffer.GetHeight();
        /* Bounding Box Setup */
        Vec4 fragCoords[3];
        fragCoords[0] = varyings[0].FragPos;
        fragCoords[1] = varyings[1].FragPos;
        fragCoords[2] = varyings[2].FragPos;
        BoundingBox bBox = GetBoundingBox(fragCoords, width, height);

        for (int y = bBox.MinY; y <= bBox.MaxY; y++)
        {
            for (int x = bBox.MinX; x <= bBox.MaxX; x++)
            {
                /* Varyings Setup */
                float screenWeights[3];
                float weights[3];
                Vec2 screenPoint{ (float)x + 0.5f, (float)y + 0.5f };

                CalculateWeights(screenWeights, weights, fragCoords, screenPoint);
                if (!IsInsideTriangle(weights))
                    continue;

                varyings_t pixVaryings;
                LerpVaryings(pixVaryings, varyings, weights, width, height);

                /* Early Depth Test (深度测试) */
                if (program.EnableDepthTest)
                {
                    float depth = pixVaryings.FragPos.Z;
                    float fDepth = framebuffer.GetDepth(x, y);
                    DepthFuncType depthFunc = program.DepFunc;
                    if (!PassDepthTest(depth, fDepth, depthFunc))
                    {
                        continue;
                    }
                }

                /* Pixel Processing */
                ProcessPixel(framebuffer, x, y, program, pixVaryings, uniforms);
            }
        }
    }

public:
    /**
     * @brief 绘制
     * @param framebuffer 帧缓存
     * @param program 着色器程序
     * @param triangle 三角形
     * @param uniforms 统一变量
    */
    template<typename vertex_t, typename uniforms_t, typename varyings_t>
    static void Draw(Framebuffer& framebuffer,
                    const Program<vertex_t, uniforms_t, varyings_t>& program,
                    const Triangle<vertex_t>& triangle,
                    const uniforms_t& uniforms)
    {
        static_assert(std::is_base_of_v<VertexBase, vertex_t>, "vertex_t 必须继承自 RGS::VertexBase");
        static_assert(std::is_base_of_v<VaryingsBase, varyings_t>, "varyings_t 必须继承自 RGS::VaryingsBase");

        /* Vertex Shading & Projection */
        varyings_t varyings[RGS_MAX_VARYINGS];
        for (int i = 0; i < 3; i++)
        {
            program.VertexShader(varyings[i], triangle[i], uniforms);
        }

        /* Clipping */
        int vertexNum = Clip(varyings);

        /* Screen Mapping */
        CaculateNdcPos(varyings, vertexNum);
        int fWidth = framebuffer.GetWidth();
        int fHeight = framebuffer.GetHeight();
        CaculateFragPos(varyings, vertexNum, (float)fWidth, (float)fHeight);

        /* Triangle Assembly & Rasterization */
        for (int i = 0; i < vertexNum - 2; i++)
        {
            varyings_t triVaryings[3];
            triVaryings[0] = varyings[0];
            triVaryings[1] = varyings[i + 1];
            triVaryings[2] = varyings[i + 2];

            RasterizeTriangle(framebuffer, program, triVaryings, uniforms);
        }
    }
};

}