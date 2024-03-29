#include <cstdint>
#include <cstring>
#include <memory>
#include <AppKit/NSWindow.h>
#include <Cocoa/Cocoa.h>

#include "../Window.h"

@interface WindowDelegate : NSObject<NSWindowDelegate>
{
}

@property (readonly, assign) tg::Window* window;
-(id)initWithWindow:(tg::Window*)window;
@end

@implementation WindowDelegate
-(id)initWithWindow:(tg::Window*)window
{
    self = [super init];
    if (self)
    {
        _window = window;
    }
    
    return self;
}

-(void)windowDidResize:(NSNotification*)notification
{
    if (_window->OnResize != nullptr)
    {
        const auto extent = _window->GetClientSize();
        _window->OnResize(extent.width, extent.height);
    }
}

-(void)windowDidMove:(NSNotification*)notification
{
    if (_window->OnMove != nullptr)
    {
        const auto pos = _window->GetPosition();
        _window->OnMove(pos.x, pos.y);
    }
}

-(void)windowWillMiniaturize:(NSNotification*)notification
{
    if (_window->OnMinimize != nullptr)
    {
        _window->OnMinimize();
    }
}

-(void)windowDidDeminiaturize:(NSNotification*)notification
{
    if (_window->OnMaximize != nullptr)
    {
        _window->OnMaximize();
    }
}

-(void)windowDidEnterFullScreen:(NSNotification *)notification
{
    if (_window->OnEnterFullScreen != nullptr)
    {
        _window->OnEnterFullScreen();
    }
}

-(void)windowDidExitFullScreen:(NSNotification *)notification
{
    if (_window->OnExitFullScreen != nullptr)
    {
        _window->OnExitFullScreen();
    }
}
@end

TG_NAMESPACE_BEGIN
namespace
{

constexpr NSWindowStyleMask ConvertWindowStyleToNative(const WindowStyle& windowStyle) noexcept
{
    NSWindowStyleMask nativeWindowStyleMask = {};
    
    if (windowStyle.hasCaption == true)
    {
        nativeWindowStyleMask |= NSWindowStyleMaskTitled;
    }
    else
    {
        nativeWindowStyleMask = NSWindowStyleMaskBorderless;
    }
    
    if (windowStyle.resizeable == true)
    {
        nativeWindowStyleMask |= NSWindowStyleMaskResizable;
    }
    
    if (windowStyle.enableSystemButton == true)
    {
        nativeWindowStyleMask |= NSWindowStyleMaskClosable | NSWindowStyleMaskMiniaturizable;
    }
    
    return nativeWindowStyleMask;
}

NSWindow* CreateNativeWindow(const WindowStyle& windowStyle)
{
    NSScreen* mainScreen = [NSScreen mainScreen];
    
    // Create a window.
    NSWindow* window = [[NSWindow alloc] initWithContentRect:NSMakeRect(0.0, 0.0, 100.0, 100.0)
                                                   styleMask:ConvertWindowStyleToNative(windowStyle)
                                                     backing:NSBackingStoreBuffered
                                                       defer:NO
                                                      screen:mainScreen];
    
    // Set window attributes.
    [window setAcceptsMouseMovedEvents:YES];
    [window makeKeyAndOrderFront:[NSApplication sharedApplication]];
    [window setBackgroundColor:[NSColor whiteColor]];
    [window setReleasedWhenClosed:YES];
    [window setLevel:NSMainMenuWindowLevel];
    [window setTitle:[NSString stringWithUTF8String:reinterpret_cast<const char*>(windowStyle.title.c_str())]];
    
    if (windowStyle.enableSystemButton == false)
    {
        [[window standardWindowButton:NSWindowZoomButton] setEnabled:NO];
    }
    
    // Set the window content size.
    [window setContentSize:NSMakeSize(CGFloat(windowStyle.width), CGFloat(windowStyle.height))];
    
    // Set the window position.
    NSRect windowRect = [window frame];
    NSRect mainScreenRect = [[NSScreen mainScreen] visibleFrame];
    NSPoint windowPosition;
    if (windowStyle.showMiddle == true)
    {
        windowPosition = NSMakePoint((mainScreenRect.size.width * 0.5f) - (windowRect.size.width * 0.5f), ((mainScreenRect.origin.y + mainScreenRect.size.height) * 0.5f) - (windowRect.size.height * 0.5f));
    }
    else
    {
        windowPosition = NSMakePoint(CGFloat(windowStyle.x), (mainScreenRect.origin.y + mainScreenRect.size.height - windowRect.size.height) - CGFloat(windowStyle.y));
    }
    [window setFrameOrigin:windowPosition];
    
    return window;
}
    
}

MacOSWindow::MacOSWindow(NSWindow* window, WindowDelegate* windowDelegate) noexcept :
    _window(window),
    _windowDelegate(windowDelegate)
{
}

MacOSWindow::MacOSWindow(MacOSWindow&& rhs) noexcept :
    _window(rhs._window),
    _windowDelegate(rhs._windowDelegate)
{
    rhs._window = nullptr;
    rhs._windowDelegate = nullptr;
}

MacOSWindow& MacOSWindow::operator=(MacOSWindow&& rhs) noexcept
{
    _window = rhs._window;
    _windowDelegate = rhs._windowDelegate;
    
    rhs._window = nullptr;
    rhs._windowDelegate = nullptr;
    
    return *this;
}

Window::Window(const WindowStyle& windowStyle) :
    MacOSWindow(CreateNativeWindow(windowStyle), [[WindowDelegate alloc] initWithWindow:reinterpret_cast<Window*>(this)])
{
}

void Window::Show()
{
    [_window display];
}

void Window::Hide()
{
    [_window miniaturize:nullptr];
}

void Window::Close()
{
    [_window close];

    _window = nullptr;
}

void Window::Maximize()
{
    [_window zoom:nullptr];
}

void Window::Minimize()
{
    [_window miniaturize:nullptr];
}
    
void Window::BringToFront()
{
    [NSApp activateIgnoringOtherApps:YES];
    [NSApp activateIgnoringOtherApps:NO];
}

void Window::SetPosition(int32_t x, int32_t y)
{
    NSRect mainScreenRect = [[NSScreen mainScreen] visibleFrame];
    NSRect windowRect = [_window frame];

    [_window setFrameOrigin:NSMakePoint(static_cast<CGFloat>(x),(mainScreenRect.origin.y + mainScreenRect.size.height - windowRect.size.height) - y)];
}

void Window::SetContentSize(int32_t width, int32_t height)
{
    NSRect windowRect = [_window frame];
    windowRect.size.width = static_cast<CGFloat>(width);
    windowRect.size.height = static_cast<CGFloat>(height);
    
    NSRect clientRect = [_window frameRectForContentRect:windowRect];
    clientRect.origin.y += clientRect.size.height - static_cast<CGFloat>(height);
    
    [_window setContentSize:NSMakeSize(clientRect.size.width, clientRect.size.height)];
}

void Window::SetTitle(const char8_t* title)
{
    [_window setTitle:[NSString stringWithUTF8String:reinterpret_cast<const char*>(title)]];
}

void Window::SetTopMost(bool setTopMost)
{
    NSWindowLevel windowLevel = (setTopMost) ? NSStatusWindowLevel : NSMainMenuWindowLevel;
    [_window setLevel:windowLevel];
}

bool Window::IsTopMost() const
{
    return [_window level] == NSStatusWindowLevel;
}

void Window::SetTransparency(float transparency)
{
    [_window setAlphaValue:transparency];
}

IntVector2 Window::GetPosition() const
{
    NSRect mainScreenRect = [[NSScreen mainScreen] visibleFrame];
    NSRect windowRect = [_window frame];
    return {static_cast<int32_t>(windowRect.origin.x), static_cast<int32_t>((mainScreenRect.origin.y + mainScreenRect.size.height - windowRect.size.height) - windowRect.origin.y)};
}

I32Extent2D Window::GetWindowSize() const
{
    auto windowRect = [_window frame];
    return {static_cast<int32_t>(windowRect.size.width), static_cast<int32_t>(windowRect.size.height)};
}

I32Extent2D Window::GetClientSize() const
{
    auto windowRect = [[_window contentView] frame];
    return {static_cast<int32_t>(windowRect.size.width), static_cast<int32_t>(windowRect.size.height)};
}

int32_t Window::GetTitle(char8_t* destTitle, int32_t destTitleBufferLen) const
{
    auto title = [_window title];
    auto titleLen = static_cast<int32_t>([title lengthOfBytesUsingEncoding:NSUTF8StringEncoding]);
    if (titleLen + 1 > destTitleBufferLen)
    {
        return 0;
    }
    
    memcpy(destTitle, [title UTF8String], titleLen + 1);
    return titleLen;
}

void* Window::GetNativeWindow() const
{
    return (__bridge void*)(_window);
}

float Window::GetTransparency() const
{
    return static_cast<float>([_window alphaValue]);
}

bool Window::HasCaption() const
{
    return static_cast<bool>([_window hasTitleBar]);
}

bool Window::IsResizable() const
{
    return static_cast<bool>([_window isResizable]);
}

bool Window::IsMaximized() const
{
    return static_cast<bool>([_window isZoomed]);
}

bool Window::IsMinimized() const
{
    return static_cast<bool>([_window isMiniaturized]);
}

}
