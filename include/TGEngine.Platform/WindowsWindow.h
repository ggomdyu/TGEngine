#pragma once

#include "Windows.h"

TG_NAMESPACE_BEGIN

class WindowsWindow
{
public:
    explicit WindowsWindow(HWND wndHandle) noexcept;
    WindowsWindow(const WindowsWindow& rhs) = delete;
    WindowsWindow(WindowsWindow&& rhs) noexcept;
    ~WindowsWindow() = default;

public:
    WindowsWindow& operator=(const WindowsWindow& rhs) = delete;
    WindowsWindow& operator=(WindowsWindow&& rhs) noexcept;

public:
    void SetNativeWindowStyle(DWORD nativeWindowStyle);
    void SetNativeExtendedWindowStyle(DWORD nativeExtendedWindowStyle);
    void SetUserData(void* data);
    void* GetUserData();
    const void* GetUserData() const;
    DWORD GetNativeWindowStyle() const;
    DWORD GetNativeExtendedWindowStyle() const;

protected:
    HWND _wndHandle;
};

using PlatformWindow = WindowsWindow;

}