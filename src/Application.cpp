#include <chrono>
#include <cmath>
#include <cstdio>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>

#include "Application.h"
#include "RGS/Base.h"
#include "RGS/Framebuffer.h"
#include "RGS/InputCodes.h"
#include "RGS/Texture.h"
#include "RGS/Window.h"
#include "RGS/Maths.h"
#include "RGS/Shaders/BlinnShader.h"
#include "RGS/Renderer.h"
using namespace RGS;

Application::Application(const std::string name, const int width, const int height)
    : m_Name(name),
    m_Width(width),
    m_Height(height)
{
    Init();
}

Application::~Application()
{
    Terminate();
}

void Application::Init()
{
    Window::Init();
    m_Window = Window::Create(m_Name, m_Width, m_Height);
    m_LastFrameTime = std::chrono::steady_clock::now();

    LoadMesh("assets/box.obj");

    m_Uniforms.Diffuse = new Texture("assets/container2.png");
    m_Uniforms.Specular = new Texture("assets/container2_specular.png");
}

void Application::Terminate()
{   
    delete m_Uniforms.Diffuse;
    delete m_Uniforms.Specular;

    delete m_Window;
    Window::Terminate();
}

void Application::Run()
{
    while (!m_Window->Closed()) 
    {
        /* 计算帧率 */
        auto nowFrameTime = std::chrono::steady_clock::now();       // 获取当前时间
        auto duration = std::chrono::duration_cast<std::chrono::microseconds>(nowFrameTime - m_LastFrameTime);      // 计算时间差
        float deltaTime = duration.count() * 0.001f * 0.001f;       // 转换为秒，并乘以缩放系数
        m_LastFrameTime = nowFrameTime;      // 更新上一帧时间

        OnUpdate(deltaTime);

        Window::PollInputEvents();
    }
}

void Application::LoadMesh(const char* filename)
{
    // TODO: 加载网格数据
    std::ifstream file(filename);
    ASSERT(file);

    std::vector<Vec3> positions;        // 顶点信息
    std::vector<Vec2> texCoords;;       // 纹理信息
    std::vector<Vec3> normals;          // 法线信息
    std::vector<int> posIndices;        // 顶点索引
    std::vector<int> texIndices;        // 纹理索引
    std::vector<int> normalIndices;     // 法线索引

    std::string line;
    while (!file.eof()) 
    {
        std::getline(file, line);
        int items = -1;     // 记录每行有多少项
        if (line.find("v ") == 0)                   /* Position */
        {
            Vec3 position;
            items = std::sscanf(line.c_str(), "v %f %f %f", 
                    &position.X, &position.Y, &position.Z);     // 读取三维坐标
            ASSERT(items == 3);     // 确保读取成功(三个顶点)
            positions.push_back(position);  // 加入顶点列表
        }
        else if (line.find("vt ") == 0)             /* Texcoord */
        {
            Vec2 texcoord;
            items = std::sscanf(line.c_str(), "vt %f %f", 
                    &texcoord.X, &texcoord.Y);     // 读取二维纹理坐标
            ASSERT(items == 2);
            texCoords.push_back(texcoord);
        }
        else if (line.find("vn ") == 0)             /* Normal */ 
        {
            Vec3 normal;
            items = std::sscanf(line.c_str(), "vn %f %f %f", 
                    &normal.X, &normal.Y, &normal.Z);     // 读取三维法线
            ASSERT(items == 3);
            normals.push_back(normal);
        }
        else if (line.find("f ") == 0)              /* Face */
        {
            int pIndices[3], uvIndices[3], nIndices[3];     
            items = std::sscanf(line.c_str(), "f %d/%d/%d %d/%d/%d %d/%d/%d",
                    &pIndices[0], &uvIndices[0], &nIndices[0],
                    &pIndices[1], &uvIndices[1], &nIndices[1],
                    &pIndices[2], &uvIndices[2], &nIndices[2]);
            ASSERT(items == 9);
            for (int i = 0; i < 3; i++)
            {
                posIndices.push_back(pIndices[i] - 1);      // 顶点索引从1开始
                texIndices.push_back(uvIndices[i] - 1);     // 纹理索引从1开始
                normalIndices.push_back(nIndices[i] - 1);   // 法线索引从1开始
            }
        }
    }
    file.close();       // 关闭文件

    int triNum = posIndices.size() / 3;     // 三角形数量
    for (int i = 0; i < triNum; i++)
    {
        Triangle<BlinnVertex> triangle;
        for (int j = 0; j < 3; j++)
        {
            int index = 3 * i + j;
            int posIndex = posIndices[index];
            int texIndex = texIndices[index];
            int nlIndex = normalIndices[index];
            triangle[j].ModelPos = { positions[posIndex], 1.0f };
            triangle[j].TexCoord = texCoords[texIndex];
            triangle[j].ModelNormal = normals[nlIndex];
        }
        m_Mesh.emplace_back(triangle);
    }
}

void Application::OnCameraUpdate(float time)
{
    /* 移动 */
    constexpr float speed = 1.0f;
    if (m_Window->GetKey(RGS_KEY_SPACE) == RGS_PRESS)
        m_Camera.Pos = m_Camera.Pos + speed * time * m_Camera.Up;
    if (m_Window->GetKey(RGS_KEY_LEFT_SHIFT) == RGS_PRESS)
        m_Camera.Pos = m_Camera.Pos - speed * time * m_Camera.Up;
    if (m_Window->GetKey(RGS_KEY_D) == RGS_PRESS)
        m_Camera.Pos = m_Camera.Pos + speed * time * m_Camera.Right;
    if (m_Window->GetKey(RGS_KEY_A) == RGS_PRESS)
        m_Camera.Pos = m_Camera.Pos - speed * time * m_Camera.Right;
    if (m_Window->GetKey(RGS_KEY_W) == RGS_PRESS)
        m_Camera.Pos = m_Camera.Pos + speed * time * m_Camera.Dir;
    if (m_Window->GetKey(RGS_KEY_S) == RGS_PRESS)
        m_Camera.Pos = m_Camera.Pos - speed * time * m_Camera.Dir;

    /* 旋转（绕y轴）*/
    constexpr float rotateSpeed = 1.0f;
    Mat4 rotation = Mat4Identity();         // 单位矩阵
    if (m_Window->GetKey(RGS_KEY_Q) == RGS_PRESS)
        rotation = Mat4RotateY(time * rotateSpeed);
    if (m_Window->GetKey(RGS_KEY_E) == RGS_PRESS)
        rotation = Mat4RotateY(-time * rotateSpeed);
    m_Camera.Dir = rotation * m_Camera.Dir;
    m_Camera.Dir = { Normalize(m_Camera.Dir), 0.0f };
    m_Camera.Right = rotation * m_Camera.Right;
    m_Camera.Right = { Normalize(m_Camera.Right), 0.0f };
}

void Application::OnUpdate(float time)
{
    OnCameraUpdate(time);

    Framebuffer framebuffer(m_Width, m_Height);
    Program program(BlinnVertexShader, BlinnFragmentShader);

    Mat4 view = Mat4LookAt(m_Camera.Pos, m_Camera.Pos + m_Camera.Dir, {0.0f, 1.0f, 0.0f});
    Mat4 proj = Mat4Perspective(90.0f / 360.0f * 2.0f * PI, m_Camera.Aspect, 0.1f, 100.0f);

    Mat4 model = Mat4Identity();
    m_Uniforms.MVP = proj * view * model;
    m_Uniforms.CameraPos = m_Camera.Pos;
    m_Uniforms.Model = model;
    m_Uniforms.ModelNormalToWorld = Mat4Identity();

    m_Uniforms.Shininess *= std::pow(2, time * 2.0f);
    if (m_Uniforms.Shininess > 256.0f)
        m_Uniforms.Shininess -= 256.0f;

    for (auto tri : m_Mesh)
    {
        Renderer::Draw(framebuffer, program, tri, m_Uniforms);
    }

    m_Window->DrawFramebuffer(framebuffer);
}