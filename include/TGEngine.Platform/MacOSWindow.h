#pragma once

@class NSWindow;
@class WindowDelegate;

TG_NAMESPACE_BEGIN

class MacOSWindow
{
protected:
    MacOSWindow(NSWindow* window, WindowDelegate* windowDelegate) noexcept;
    MacOSWindow(const MacOSWindow& rhs) = delete;
    MacOSWindow(MacOSWindow&& rhs) noexcept;
    ~MacOSWindow() = default;

public:
    MacOSWindow& operator=(const MacOSWindow& rhs) = delete;
    MacOSWindow& operator=(MacOSWindow&& rhs) noexcept;

protected:
    NSWindow* _window;
    WindowDelegate* _windowDelegate;
};
    
using PlatformWindow = MacOSWindow;

TG_NAMESPACE_END