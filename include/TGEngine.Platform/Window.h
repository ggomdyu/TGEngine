#pragma once

#include "Core/DelegateChain.h"
#include "Math/Extent.h"
#include "Math/Vector2.h"

#if TG_PLATFORM_WINDOWS
#include "Windows/WindowsWindow.h"
#elif TG_PLATFORM_MACOS
#include "MacOS/MacOSWindow.h"
#elif TG_PLATFORM_ANDROID
#include "Android/AndroidWindow.h"
#elif TG_PLATFORM_IOS
#include "IOS/IOSWindow.h"
#endif

#include "WindowStyle.h"

TG_NAMESPACE_BEGIN

class Window final :
    private PlatformWindow
{
public:
    explicit Window(const WindowStyle& windowStyle);
    Window(const Window&) = delete;
    Window(Window&&) noexcept = default;
    ~Window();

public:
    Window& operator=(const Window& rhs) = delete;
    Window& operator=(Window&& rhs) noexcept = default;
    bool operator==(const Window& rhs) const noexcept;
    bool operator!=(const Window& rhs) const noexcept;

public:
    PlatformWindow& GetPlatformDependency() noexcept;
    const PlatformWindow& GetPlatformDependency() const noexcept;
    void Show();
    void Hide();
    void Close();
    void Maximize();
    void Minimize();
    void BringToFront();
    void Flash();
    void SetPosition(int32_t x, int32_t y);
    void SetContentSize(int32_t width, int32_t height);
    void SetTitle(const char8_t* title);
    void SetTopMost(bool setTopMost);
    void SetTransparency(float transparency);
    IntVector2 GetPosition() const;
    I32Extent2D GetWindowSize() const;
    I32Extent2D GetClientSize() const;
    int32_t GetTitle(char8_t* destStr, int32_t destStrBufferLen) const;
    std::u8string GetTitle() const;
    float GetTransparency() const;
    void* GetNativeWindow() const;
    bool HasCaption() const;
    bool IsResizable() const;
    bool IsMaximized() const;
    bool IsMinimized() const;
    bool IsTopMost() const;

public:
    DelegateChain<void(int32_t, int32_t)> OnMove;
    DelegateChain<void(int32_t, int32_t)> OnResize;
    DelegateChain<void()> OnMaximize;
    DelegateChain<void()> OnMinimize;
    DelegateChain<void()> OnEnterFullScreen;
    DelegateChain<void()> OnExitFullScreen;
    DelegateChain<void()> OnWillClose;
    DelegateChain<void()> OnDidClose;
    DelegateChain<void()> OnGetFocus;
    DelegateChain<void()> OnLoseFocus;
};

}
