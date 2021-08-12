#pragma once

#include <memory>

#include "Platform/Window.h"
#if TGON_PLATFORM_WINDOWS
#include "Windows/WindowsApplication.h"
#elif TGON_PLATFORM_MACOS
#include "MacOS/MacOSApplication.h"
#elif TGON_PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#elif TGON_PLATFORM_IOS
#include "IOS/IOSApplication.h"
#endif

#include "Engine.h"
#include "MessageBoxIcon.h"

namespace tg
{

class Application final :
    private PlatformApplication
{
/**@section Constructor */
private:
    Application() = default;

/**@section Method */
public:
    [[nodiscard]] static Application& GetInstance() noexcept;
    [[nodiscard]] PlatformApplication& GetPlatformDependency() noexcept;
    [[nodiscard]] const PlatformApplication& GetPlatformDependency() const noexcept;
    void Initialize(std::unique_ptr<class Engine> engine);
    void MessageLoop();
    [[noreturn]] static void Terminate();
    static void ShowMessageBox(const char8_t* message);
    static void ShowMessageBox(const char8_t* message, MessageBoxIcon messageBoxIcon);
    static void ShowMessageBox(const char8_t* title, const char8_t* message);
    static void ShowMessageBox(const char8_t* title, const char8_t* message, MessageBoxIcon messageBoxIcon);
    [[nodiscard]] Engine& GetEngine() noexcept;
    [[nodiscard]] const Engine& GetEngine() const noexcept;
    [[nodiscard]] std::shared_ptr<Window> GetRootWindow() noexcept;
    [[nodiscard]] std::shared_ptr<const Window> GetRootWindow() const noexcept;
    
/**@section Variable */
protected:
    std::unique_ptr<Engine> m_engine;
    std::shared_ptr<Window> m_rootWindow;
};

}
