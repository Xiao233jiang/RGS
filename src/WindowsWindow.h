#pragma once

#include "Framebuffer.h"
#include "Window.h"

#include <string>
#include <Windows.h>

namespace RGS {

class WindowsWindow : public Window 
{
public:
    WindowsWindow(const std::string title, const int width, const int height);
    ~WindowsWindow();

    virtual void Show() const override;
    virtual void DrawFramebuffer(const Framebuffer& framebuffer) override;  // 绘制帧缓存

public:
    static void Init();
    static void Terminate();

    static void PollInputEvents();

private:
    static void Register();     // 注册窗口类
    static void Unregister();   // 注销窗口类

    /**
     * @brief 按键事件处理函数
     * @param window 窗口对象
     * @param wParam 附加参数1
     * @param state 按键状态
     * @return 处理结果
    */
    static void KeyPressImpl(WindowsWindow* window, const WPARAM wParam, const char state);
    /**
     * @brief 窗口过程回调函数
     * @param hWnd 窗口句柄
     * @param msgID 消息ID
     * @param wParam 附加参数1
     * @param lParam 附加参数2
     * @return 处理结果
    */
    static LRESULT CALLBACK WndProc(const HWND hWnd, const UINT msgID, const WPARAM wParam, const LPARAM lParam);

private:
    HWND m_Handle;              // 窗口句柄
    HDC m_MemoryDC;             // 内存设备上下文
    unsigned char* m_Buffer;    // 缓冲区

    static bool s_Inited;       // 窗口类是否已注册
};

}