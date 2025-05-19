#pragma once

#include <chrono>
#include <string>
#include <vector>

#include "RGS/Maths.h"
#include "RGS/Renderer.h"
#include "RGS/Shaders/BlinnShader.h"
#include "RGS/Window.h"

namespace RGS {

struct Camera 
{
    Vec4 Pos = { 0.0f, 0.0f, 0.0f, 1.0f };          // 相机位置(点)
    Vec4 Right = { 1.0f, 0.0f, 0.0f, 0.0f };        // 相机右边方向(向量)
    Vec4 Up = { 0.0f, 1.0f, 0.0f, 0.0f };           // 相机上方向(向量)
    Vec4 Dir = { 0.0f, 0.0f, -1.0f, 0.0f };         // 相机视线方向(向量)
    float Aspect = 4.0f / 3.0f;                               // 宽高比
    float Fovy = 45.0f;                                        // 视角
};

class Application
{
public:
    Application(const std::string name, const int width, const int height);
    ~Application();

    void Run();

private:
    void Init();        // 构造调用
    void Terminate();   // 析构调用

    void OnCameraUpdate(float time);    
    void OnUpdate(float time);

    void LoadMesh(const char* filename);

private:
    std::string m_Name;
    int m_Width;
    int m_Height;
    std::chrono::steady_clock::time_point m_LastFrameTime;      // 上一帧时间

    Window* m_Window;     // 
    Camera m_Camera;      // 相机

    std::vector<Triangle<BlinnVertex>> m_Mesh;      // 网格

    BlinnUniforms m_Uniforms;       // 着色器参数
};

}