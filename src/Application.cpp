#include <iostream>
#include <string>

#include "Application.h"
#include "RGS/Framebuffer.h"
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
}

void Application::Terminate()
{   
    delete m_Window;
    Window::Terminate();
}

void Application::Run()
{
    while (!m_Window->Closed()) 
    {
        OnUpdate();

        Window::PollInputEvents();
    }
}

void Application::OnUpdate()
{
    if (m_Window->GetKey(RGS_KEY_0) == RGS_PRESS)
        std::cout << "0 按下" << std::endl;
    if (m_Window->GetKey(RGS_KEY_B) == RGS_PRESS)
        std::cout << "B 按下" << std::endl;
    if (m_Window->GetKey(RGS_KEY_A) == RGS_PRESS)
        std::cout << "A 按下" << std::endl;
    if (m_Window->GetKey(RGS_KEY_O) == RGS_PRESS)
        std::cout << "O 按下" << std::endl;

    if (m_Window->GetKey(RGS_KEY_R) == RGS_PRESS)
        std::cout << "R 按下" << std::endl;
    if (m_Window->GetKey(RGS_KEY_G) == RGS_PRESS)
        std::cout << "G 按下" << std::endl;
    if (m_Window->GetKey(RGS_KEY_S) == RGS_PRESS)
        std::cout << "S 按下" << std::endl;

    Framebuffer framebuffer(m_Width, m_Height);
    framebuffer.Clear( {0.0f, 0.0f, 0.0f} );       // 用 Vec3 定义颜色清屏

    Program program(BlinnVertexShader, BlinnFragmentShader);
    Triangle<BlinnVertex> tri;
    tri[0].ModelPos = { 0.0f, 0.0f, -8.0f, 1.0f };
    tri[1].ModelPos = { -10.0f, -10.0f, -10.0f, 1.0f };
    tri[2].ModelPos = { 30.0f, -10.0f, -10.0f, 1.0f };
    BlinnUniforms uniforms;
    uniforms.MVP = Mat4Perspective(90.0f / 180.0f * PI, 1.0f, 1.0f, 10.0f);

    Renderer::Draw(framebuffer, program, tri, uniforms);

    m_Window->DrawFramebuffer(framebuffer);
}