#pragma once

#include <string>

#include "RGS/Window.h"

namespace RGS {

class Application
{
public:
    Application(const std::string name, const int width, const int height);
    ~Application();

    void Run();

private:
    void Init();        // 构造调用
    void Terminate();   // 析构调用

    void OnUpdate();

private:
    std::string m_Name;
    int m_Width;
    int m_Height;

    Window* m_Window;
};

}