#include <iostream>
#include <string>

#include "Application.h"
#include "Framebuffer.h"
#include "Window.h"
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
        std::cout << "0 被按下" << std::endl;
    if (m_Window->GetKey(RGS_KEY_A) == RGS_PRESS)
        std::cout << "A 被按下" << std::endl;

    Framebuffer framebuffer(m_Width, m_Height);
    framebuffer.Clear( {0.5f, 0.2f, 0.01f} );
    m_Window->DrawFramebuffer(framebuffer);
}