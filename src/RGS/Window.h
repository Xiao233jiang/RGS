#pragma once

#include <cstdint>
#include <string>

#include "Framebuffer.h"
#include "InputCodes.h"

namespace RGS {

class Window 
{
public:
    Window(const std::string title, const int width, const int height);
    virtual ~Window() {};

    virtual void Show() const = 0;
    virtual void DrawFramebuffer(const Framebuffer& framebuffer) = 0;   // 绘制帧缓存内容

    bool Closed() const { return m_Closed; }
    char GetKey(const uint32_t index) const { return m_Keys[index]; }

public:
    static void Init();         // 构造时调用
    static void Terminate();    // 析构时调用
    static Window* Create(const std::string title, const int width, const int height);

    static void PollInputEvents();  // 轮询输入事件

protected:
    std::string m_Title;
    int m_Width;
    int m_Height;
    bool m_Closed = true;   // 窗口关闭标志

    char m_Keys[RGS_KEY_MAX_COUNT];     // 键盘输入状态
};

}