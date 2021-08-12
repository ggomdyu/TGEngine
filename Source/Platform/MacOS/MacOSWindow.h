#pragma once

@class NSWindow;
@class WindowDelegate;

namespace tg
{

class MacOSWindow
{
/**@section Constructor */
protected:
    MacOSWindow(NSWindow* window, WindowDelegate* windowDelegate) noexcept;
    MacOSWindow(const MacOSWindow& rhs) = delete;
    MacOSWindow(MacOSWindow&& rhs) noexcept;

/**@section Destructor */
public:
    ~MacOSWindow() = default;

/**@section Operator */
public:
    MacOSWindow& operator=(const MacOSWindow& rhs) = delete;
    MacOSWindow& operator=(MacOSWindow&& rhs) noexcept;

/**@section Variable */
protected:
    NSWindow* m_window;
    WindowDelegate* m_windowDelegate;
};
    
using PlatformWindow = MacOSWindow;

}