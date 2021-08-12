#pragma once

#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN
#endif

#include "Windows.h"

namespace tg
{

class WindowsWindow
{
/**@section Constructor */
public:
    explicit WindowsWindow(HWND wndHandle) noexcept;
    WindowsWindow(const WindowsWindow& rhs) = delete;
    WindowsWindow(WindowsWindow&& rhs) noexcept;

/**@section Destructor */
public:
    ~WindowsWindow() = default;

/**@section Operator */
public:
    WindowsWindow& operator=(const WindowsWindow& rhs) = delete;
    WindowsWindow& operator=(WindowsWindow&& rhs) noexcept;

/**@section Method */
public:
    void SetNativeWindowStyle(DWORD nativeWindowStyle);
    void SetNativeExtendedWindowStyle(DWORD nativeExtendedWindowStyle);
    void SetUserData(void* data);
    [[nodiscard]] void* GetUserData();
    [[nodiscard]] const void* GetUserData() const;
    [[nodiscard]] DWORD GetNativeWindowStyle() const;
    [[nodiscard]] DWORD GetNativeExtendedWindowStyle() const;

/**@section Variable */
protected:
    HWND m_wndHandle;
};

using PlatformWindow = WindowsWindow;

}