#pragma once

#include "Core/DelegateChain.h"
#include "Math/Extent.h"
#include "Math/Vector2.h"

#if TGON_PLATFORM_WINDOWS
#include "Windows/WindowsWindow.h"
#elif TGON_PLATFORM_MACOS
#include "MacOS/MacOSWindow.h"
#elif TGON_PLATFORM_ANDROID
#include "Android/AndroidWindow.h"
#elif TGON_PLATFORM_IOS
#include "IOS/IOSWindow.h"
#endif

#include "WindowStyle.h"

namespace tg
{

class Window final :
    private PlatformWindow
{
/**@section Constructor */
public:
    explicit Window(const WindowStyle& windowStyle);
    Window(const Window&) = delete;
    Window(Window&&) noexcept = default;

/**@section Destructor */
public:
    ~Window();

/**@section Operator */
public:
    Window& operator=(const Window& rhs) = delete;
    Window& operator=(Window&& rhs) noexcept = default;
    bool operator==(const Window& rhs) const noexcept;
    bool operator!=(const Window& rhs) const noexcept;

/**@section Method */
public:
    [[nodiscard]] PlatformWindow& GetPlatformDependency() noexcept;
    [[nodiscard]] const PlatformWindow& GetPlatformDependency() const noexcept;
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
    [[nodiscard]] IntVector2 GetPosition() const;
    [[nodiscard]] I32Extent2D GetWindowSize() const;
    [[nodiscard]] I32Extent2D GetClientSize() const;
    int32_t GetTitle(char8_t* destStr, int32_t destStrBufferLen) const;
    [[nodiscard]] std::u8string GetTitle() const;
    [[nodiscard]] float GetTransparency() const;
    [[nodiscard]] void* GetNativeWindow() const;
    [[nodiscard]] bool HasCaption() const;
    [[nodiscard]] bool IsResizable() const;
    [[nodiscard]] bool IsMaximized() const;
    [[nodiscard]] bool IsMinimized() const;
    [[nodiscard]] bool IsTopMost() const;

/**@section Variable */
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
