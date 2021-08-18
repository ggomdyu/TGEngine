#include "PrecompiledHeader.h"

#include <sstream>
#include <tuple>
#include <array>
#include <fmt/format.h>
#if TGON_USING_DWMAPI
#include <dwmapi.h>
#pragma comment(lib, "dwmapi.lib")
#endif

#include "Diagnostics/Debug.h"

#include "../Window.h"

namespace tg
{
namespace
{

constexpr std::tuple<DWORD, DWORD> ConvertWindowStyleToNative(const WindowStyle& windowStyle) noexcept
{
    DWORD nativeWindowStyle = 0;
    DWORD nativeExtendedWindowStyle = 0;

    // Create a normal window style.
    {
        nativeWindowStyle |= WS_VISIBLE;

        if (windowStyle.resizeable)
        {
            nativeWindowStyle |= WS_THICKFRAME;
        }

        if (!windowStyle.hasCaption)
        {
            nativeWindowStyle |= WS_POPUP;
        }
        else
        {
            nativeWindowStyle |= WS_OVERLAPPED;
            nativeWindowStyle |= WS_CAPTION;
        }

        if (windowStyle.enableSystemButton)
        {
            nativeWindowStyle |= WS_SYSMENU;
            nativeWindowStyle |= WS_MINIMIZEBOX;
            nativeWindowStyle |= WS_MAXIMIZEBOX;
        }
    }

    // Create a extended window style.
    {
        if (windowStyle.topMost)
        {
            nativeExtendedWindowStyle |= WS_EX_TOPMOST;
        }
    }

    return {nativeWindowStyle , nativeExtendedWindowStyle};
}

HWND CreateNativeWindow(const WindowStyle& windowStyle, HINSTANCE instanceHandle, const wchar_t* className = L"TGON", void* extraParam = nullptr)
{
    IntVector2 windowPos(windowStyle.x, windowStyle.y);
    if (windowStyle.showMiddle)
    {
        // Set window position to the middle of the screen.
        windowPos.x = (GetSystemMetrics(SM_CXSCREEN) - windowStyle.width) / 2;
        windowPos.y = (GetSystemMetrics(SM_CYSCREEN) - windowStyle.height) / 2;
    }

    std::array<wchar_t, 4096> utf16Title{};
    if (MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(windowStyle.title.data()), -1, &utf16Title[0], static_cast<int>(utf16Title.size())) == 0)
    {
        return {};
    }

    auto [nativeWindowStyle, nativeExtendedWindowStyle] = ConvertWindowStyleToNative(windowStyle);

    // Convert client size to window size.
    RECT windowSize {0, 0, windowStyle.width, windowStyle.height};
    AdjustWindowRect(&windowSize, nativeWindowStyle, FALSE);

    auto* const wndHandle = ::CreateWindowExW(
        nativeExtendedWindowStyle,
        className,
        &utf16Title[0],
        nativeWindowStyle | WS_CLIPSIBLINGS | WS_CLIPCHILDREN,
        windowPos.x,
        windowPos.y,
        windowSize.right - windowSize.left,
        windowSize.bottom - windowSize.top,
        nullptr,
        nullptr,
        instanceHandle,
        extraParam
	);
    return wndHandle;
}

}

WindowsWindow::WindowsWindow(HWND wndHandle) noexcept :
    m_wndHandle(wndHandle)
{
}

WindowsWindow::WindowsWindow(WindowsWindow&& rhs) noexcept :
    m_wndHandle(rhs.m_wndHandle)
{
    rhs.m_wndHandle = nullptr;
}

Window::Window(const WindowStyle& windowStyle) :
    PlatformWindow(CreateNativeWindow(windowStyle, GetModuleHandle(nullptr), L"TGON", this))
{
    this->SetUserData(this);
}

bool Window::operator==(const Window& rhs) const noexcept
{
    return m_wndHandle == rhs.m_wndHandle;
}

bool Window::operator!=(const Window& rhs) const noexcept
{
    return m_wndHandle != rhs.m_wndHandle;
}

WindowsWindow& WindowsWindow::operator=(WindowsWindow&& rhs) noexcept
{
    std::swap(m_wndHandle, rhs.m_wndHandle);
    
    return *this;
}

void WindowsWindow::SetNativeWindowStyle(DWORD nativeWindowStyle)
{
    SetWindowLongPtrW(m_wndHandle, GWL_STYLE, nativeWindowStyle);
}

void WindowsWindow::SetNativeExtendedWindowStyle(DWORD nativeExtendedWindowStyle)
{
    SetWindowLongPtrW(m_wndHandle, GWL_EXSTYLE, nativeExtendedWindowStyle);
}

DWORD WindowsWindow::GetNativeWindowStyle() const
{
    return static_cast<DWORD>(GetWindowLongPtrW(m_wndHandle, GWL_STYLE));
}

DWORD WindowsWindow::GetNativeExtendedWindowStyle() const
{
    return static_cast<DWORD>(GetWindowLongPtrW(m_wndHandle, GWL_EXSTYLE));
}

void WindowsWindow::SetUserData(void* data)
{
    SetWindowLongPtrW(m_wndHandle, GWLP_USERDATA, reinterpret_cast<LONG_PTR>(data));
}

void* WindowsWindow::GetUserData()
{
    return reinterpret_cast<void*>(GetWindowLongPtrW(m_wndHandle, GWLP_USERDATA));
}

const void* WindowsWindow::GetUserData() const
{
    return const_cast<WindowsWindow*>(this)->GetUserData();
}

void Window::BringToFront()
{
    const auto isTopMost = this->IsTopMost();
    this->SetTopMost(true);
    this->SetTopMost(isTopMost);
}

void Window::Flash()
{
    FLASHWINFO fwi
    {
        .cbSize = sizeof(FLASHWINFO),
        .hwnd = m_wndHandle,
        .dwFlags = FLASHW_CAPTION,
        .uCount = 1,
        .dwTimeout = 0,
    };

    FlashWindowEx(&fwi);
}

IntVector2 Window::GetPosition() const
{
    RECT rt;
    GetWindowRect(m_wndHandle, &rt);

    return {rt.left, rt.top};
}

I32Extent2D Window::GetWindowSize() const
{
    RECT rt;
    GetWindowRect(m_wndHandle, &rt);

    return {rt.right, rt.bottom};
}

I32Extent2D Window::GetClientSize() const
{
    RECT rt;
    GetClientRect(m_wndHandle, &rt);

    return {rt.right, rt.bottom};
}

int32_t Window::GetTitle(char8_t* destStr, int32_t destStrBufferLen) const
{
    std::array<wchar_t, 4096> utf16Title{};
    const auto utf16TitleLen = GetWindowTextW(m_wndHandle, &utf16Title[0], 256);
    if (utf16TitleLen == 0)
    {
        return {};
    }

    const auto utf8TitleLen = WideCharToMultiByte(CP_UTF8, 0, &utf16Title[0], -1, reinterpret_cast<char*>(destStr), destStrBufferLen, nullptr, nullptr) - 1;
    if (utf8TitleLen == -1)
    {
        return {};
    }

    return utf8TitleLen;
}

bool Window::IsResizable() const
{
    if ((this->GetNativeWindowStyle() & WS_THICKFRAME) != 0)
    {
        return true;
    }

    if ((this->GetNativeExtendedWindowStyle() & WS_EX_DLGMODALFRAME) != 0)
    {
        return true;
    }

    return false;
}

bool Window::HasCaption() const
{
    return (this->GetNativeWindowStyle() & WS_CAPTION) != 0;
}

bool Window::IsMaximized() const
{
    // todo : impl
    return false;
}

bool Window::IsMinimized() const
{
    // todo : impl
    return false;
}

bool Window::IsTopMost() const
{
    return (this->GetNativeExtendedWindowStyle() & WS_EX_TOPMOST) != 0;
}

void* Window::GetNativeWindow() const
{
    return m_wndHandle;
}

void Window::Show()
{
    ShowWindow(m_wndHandle, SW_NORMAL);
}

void Window::Hide()
{
    ShowWindow(m_wndHandle, SW_HIDE);
}

void Window::Maximize()
{
    ShowWindow(m_wndHandle, SW_MAXIMIZE);
}

void Window::Minimize()
{
    ShowWindow(m_wndHandle, SW_MINIMIZE);
}

void Window::Close()
{
    if (m_wndHandle != nullptr)
    {
        DestroyWindow(m_wndHandle);
        m_wndHandle = nullptr;
    }
}

void Window::SetPosition(int32_t x, int32_t y)
{
    SetWindowPos(m_wndHandle, nullptr, x, y, 0, 0, SWP_NOSIZE | SWP_NOZORDER);
}

void Window::SetContentSize(int32_t width, int32_t height)
{
    const auto nativeWindowStyle = this->GetNativeWindowStyle();
    const auto nativeExtendedWindowStyle = this->GetNativeExtendedWindowStyle();

    RECT rt{0, 0, width, height};
    AdjustWindowRectEx(&rt, nativeWindowStyle, GetMenu(m_wndHandle) != nullptr, nativeExtendedWindowStyle);

    if (nativeWindowStyle & WS_VSCROLL)
    {
        rt.right += GetSystemMetrics(SM_CXVSCROLL);
    }

    if (nativeWindowStyle & WS_HSCROLL)
    {
        rt.bottom += GetSystemMetrics(SM_CYVSCROLL);
    }

    SetWindowPos(m_wndHandle, nullptr, 0, 0, rt.right - rt.left, rt.bottom - rt.top, SWP_NOMOVE | SWP_NOZORDER);
}

void Window::SetTitle(const char8_t* title)
{
    std::array<wchar_t, 4096> utf16Title{};
    if (MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(title), -1, &utf16Title[0], static_cast<int>(utf16Title.size())) == 0)
    {
        return;
    }

    SetWindowTextW(m_wndHandle, &utf16Title[0]);
}

void Window::SetTopMost(bool setTopMost)
{
    SetWindowPos(m_wndHandle, setTopMost ? HWND_TOPMOST : HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
}

void Window::SetTransparency(float transparency)
{
    const auto nativeExtendedWindowStyle = this->GetNativeExtendedWindowStyle();
    if ((nativeExtendedWindowStyle & WS_EX_LAYERED) == false)
    {
        this->SetNativeExtendedWindowStyle(nativeExtendedWindowStyle | WS_EX_LAYERED);
    }

    SetLayeredWindowAttributes(m_wndHandle, 0, static_cast<BYTE>(transparency * 255.0f), LWA_ALPHA);
}

float Window::GetTransparency() const
{
    BYTE transparency;
    GetLayeredWindowAttributes(m_wndHandle, nullptr, &transparency, nullptr);

    return static_cast<float>(transparency) * (1.0f / 255.0f);
}

//void Window::SetWindowTransparencyPerPixel(const Color4f& pixel, float opacity)
//{
//#if TGON_USING_DWMAPI
//    BOOL isCompoEnabled = FALSE;
//    DwmIsCompositionEnabled(&isCompoEnabled);
//    
//    if (isCompoEnabled == TRUE)
//    {
//        MARGINS margins {-1, -1, -1, -1};
//        DwmExtendFrameIntoClientArea(m_wndHandle, &margins);
//    }
//#endif
//}

}
