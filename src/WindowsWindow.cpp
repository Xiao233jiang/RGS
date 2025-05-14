#include "Base.h"

#include "Window.h"
#include "InputCodes.h"
#include "WindowsWindow.h"

#include <Windows.h>
#include <winuser.h>

#define RGS_WINDOW_ENTRY_NAME   "Entry"     // 窗口类名
#define RGS_WINDOW_CLASS_NAME   "Class"     // 窗口类名

using namespace RGS;

bool WindowsWindow::s_Inited = false;

void WindowsWindow::Init()
{
    ASSERT(!s_Inited);      // 只能初始化一次
    Register();             // 注册窗口类
    s_Inited = true;
}

void WindowsWindow::Terminate()
{
    ASSERT(s_Inited);       // 必须先初始化
    Unregister();           // 注销窗口类
    s_Inited = false;
}

void WindowsWindow::Register()
{
    ATOM atom;                                                      // 窗口类名
    WNDCLASS wc = { 0 };                                     // 窗口类
    wc.cbClsExtra = 0;                                              // 附加的窗口类内存
    wc.cbWndExtra = 0;                                              // 附加的窗口内存
    wc.hbrBackground = (HBRUSH)(WHITE_BRUSH);                       // 背景颜色
    wc.hCursor = NULL;                                              // 默认光标
    wc.hIcon = NULL;                                                // 默认图标
    wc.hInstance = GetModuleHandle(NULL);              // 当前实例
    wc.lpfnWndProc = WindowsWindow::WndProc;                        // 窗口处理函数
    wc.lpszClassName = RGS_WINDOW_CLASS_NAME;                       // 窗口类名
    wc.style = CS_HREDRAW | CS_VREDRAW;                             // 拉伸时重新绘制
    wc.lpszMenuName = NULL;                                         // 不要菜单
    atom = RegisterClass(&wc);                           // 注册窗口类
}

void WindowsWindow::Unregister()
{
    // 注销窗口类, 释放资源, GetModuleHandle(NULL)表示当前实例
    UnregisterClass(RGS_WINDOW_CLASS_NAME, GetModuleHandle(NULL));
}

WindowsWindow::WindowsWindow(const std::string title, const int width, const int height)
    : Window(title, width, height)
{
    ASSERT((s_Inited), "未初始化");

    DWORD style = WS_CAPTION | WS_SYSMENU | WS_MINIMIZEBOX;     // 窗口样式, 带标题栏, 系统菜单, 最小化按钮
    RECT rect;              // 窗口大小
    rect.left = 0;
    rect.top = 0;
    rect.bottom = (long)height;
    rect.right = (long)width;
    AdjustWindowRect(&rect, style, false);  // 调整窗口大小, 使其包含整个窗口
    // 创建窗口, 窗口类名, 标题, 样式, 位置, 大小, 父窗口, 菜单, 实例, 参数, 创建失败返回NULL
    m_Handle = CreateWindow(RGS_WINDOW_CLASS_NAME, m_Title.c_str(), style,
                            CW_USEDEFAULT, 0, rect.right - rect.left, rect.bottom - rect.top,
                            NULL, NULL, GetModuleHandle(NULL), NULL);
    ASSERT(m_Handle != nullptr);
    m_Closed = false;
    // 设置窗口句柄和窗口对象属性, 方便后续获取
    SetProp(m_Handle, RGS_WINDOW_ENTRY_NAME, this);

    HDC windowDC = GetDC(m_Handle);     // 获取窗口设备上下文
    m_MemoryDC = CreateCompatibleDC(windowDC);  // 创建内存设备上下文

    BITMAPINFOHEADER biHeader = {};     // 位图信息头
    HBITMAP newBitmap;                  // 位图句柄
    HBITMAP oldBitmap;                  // 原位图句柄

    biHeader.biSize = sizeof(BITMAPINFOHEADER);     // 位图信息头大小
    biHeader.biWidth = ((long)m_Width);             // 位图宽度
    biHeader.biHeight = ((long)m_Height);           // 位图高度
    biHeader.biPlanes = 1;                          // 颜色平面数
    biHeader.biBitCount = 24;                       // 位深度
    biHeader.biCompression = BI_RGB;                // 压缩类型

    // 分配空间
    // https://learn.microsoft.com/zh-cn/windows/win32/api/wingdi/nf-wingdi-createdibsection
    // CreateDIBSection函数创建一个DIB（设备独立位图）对象, 该对象可以直接访问其位图数据
    newBitmap = CreateDIBSection(m_MemoryDC, (BITMAPINFO*)&biHeader, DIB_RGB_COLORS, (void**)&m_Buffer, nullptr, 0);
    ASSERT(newBitmap != nullptr);
    constexpr int channelCount = 3;     // 通道数, constexpr: 编译时常量
    int size = m_Width * m_Height * channelCount * sizeof(unsigned char);   // 位图大小
    memset(m_Buffer, 0, size);
    oldBitmap = (HBITMAP)SelectObject(m_MemoryDC, newBitmap);       // 选择新位图

    DeleteObject(oldBitmap);     // 删除原位图
    ReleaseDC(m_Handle, windowDC);  // 释放窗口设备上下文

    Show();
}

WindowsWindow::~WindowsWindow()
{
    ShowWindow(m_Handle, SW_HIDE);     // 隐藏窗口
    RemoveProp(m_Handle, RGS_WINDOW_ENTRY_NAME);    // 删除窗口对象属性
    DeleteDC(m_MemoryDC);       // 删除内存设备上下文
    DestroyWindow(m_Handle);   // 销毁窗口
}

 void WindowsWindow::KeyPressImpl(WindowsWindow* window, const WPARAM wParam, const char state)
 {
    if (wParam >= '0' && wParam <= '9')
    {
        window->m_Keys[wParam] = state;
        return;
    }

    if (wParam >= 'A' && wParam <= 'Z')
    {
        window->m_Keys[wParam] = state;
        return;
    }

    switch (wParam) 
    {
        case VK_SPACE:
            window->m_Keys[RGS_KEY_SPACE] = state;
            break;
        case VK_SHIFT:
            window->m_Keys[RGS_KEY_LEFT_SHIFT] = state;
            window->m_Keys[RGS_KEY_RIGHT_SHIFT] = state;
            break;
        default:
            break;
    }
 }

void WindowsWindow::PollInputEvents()
{
    MSG message;
    // 循环获取消息, 直到队列为空
    while (PeekMessage(&message, NULL, 0, 0, PM_REMOVE))
    {
        TranslateMessage(&message);
        DispatchMessage(&message);
    }
}

LRESULT CALLBACK WindowsWindow::WndProc(const HWND hWnd, const UINT msgID, const WPARAM wParam, const LPARAM lParam)
{
    WindowsWindow* window = (WindowsWindow*)GetProp(hWnd, RGS_WINDOW_ENTRY_NAME);  // 获取窗口对象属性
    if (window == nullptr)
        return DefWindowProc(hWnd, msgID, wParam, lParam);   // 未找到窗口对象属性, 调用默认窗口处理函数

    switch (msgID) 
    {
        case WM_DESTROY:        // 窗口销毁
            window->m_Closed = true; 
            return 0;
        case WM_KEYDOWN:        // 按下键
            KeyPressImpl(window, wParam, RGS_PRESS);
            return 0;
        case WM_KEYUP:          // 松开键
            KeyPressImpl(window, wParam, RGS_RELEASE);
            return 0;
    }
    return DefWindowProc(hWnd, msgID, wParam, lParam);  // 调用默认窗口处理函数
}

void WindowsWindow::Show() const 
{
    HDC windowDC = GetDC(m_Handle);     // 获取窗口设备上下文
    // 绘制窗口内容到内存设备上下文, SRCCOPY: 直接复制
    BitBlt(windowDC, 0, 0, m_Width, m_Height, m_MemoryDC, 0, 0, SRCCOPY);
    ShowWindow(m_Handle, SW_SHOW);     // 显示窗口
    ReleaseDC(m_Handle, windowDC);          // 释放窗口设备上下文
}

void WindowsWindow::DrawFramebuffer(const Framebuffer& framebuffer)
{
    // 尽可能显示
    const int fWidth = framebuffer.GetWidth();
    const int fHeight = framebuffer.GetHeight();
    const int width = m_Width < fWidth ? m_Width : fWidth;
    const int height = m_Height < fHeight ? m_Height : fHeight;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // 翻转RGB显示
            constexpr int channelCount = 3;
            constexpr int rChannel = 2;
            constexpr int gChannel = 1;
            constexpr int bChannel = 0;

            Vec3 color = framebuffer.GetColor(j, fHeight - 1 - i);
            const int pixStart = (i * m_Width + j) * channelCount;
            const int rIndex = pixStart + rChannel;
            const int gIndex = pixStart + gChannel;
            const int bIndex = pixStart + bChannel;

            m_Buffer[rIndex] = Float2UChar(color.X);
            m_Buffer[gIndex] = Float2UChar(color.Y);
            m_Buffer[bIndex] = Float2UChar(color.Z);
        }
    }
    Show();
}
