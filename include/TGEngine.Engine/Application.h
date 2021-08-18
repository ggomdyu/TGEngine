#pragma once

#include <memory>

#include "Platform/Window.h"
#if TG_PLATFORM_WINDOWS
#include "Windows/WindowsApplication.h"
#elif TG_PLATFORM_MACOS
#include "MacOS/MacOSApplication.h"
#elif TG_PLATFORM_ANDROID
#include "Android/AndroidApplication.h"
#elif TG_PLATFORM_IOS
#include "IOS/IOSApplication.h"
#endif

#include "Engine.h"
#include "MessageBoxIcon.h"

TG_NAMESPACE_BEGIN

class Application final :
    private PlatformApplication
{
private:
    Application() = default;

public:
    static Application& GetInstance() noexcept;
    PlatformApplication& GetPlatformDependency() noexcept;
    const PlatformApplication& GetPlatformDependency() const noexcept;
    void Initialize(std::unique_ptr<class Engine> engine);
    void MessageLoop();
    [[noreturn]] static void Terminate();
    static void ShowMessageBox(const char8_t* message);
    static void ShowMessageBox(const char8_t* message, MessageBoxIcon messageBoxIcon);
    static void ShowMessageBox(const char8_t* title, const char8_t* message);
    static void ShowMessageBox(const char8_t* title, const char8_t* message, MessageBoxIcon messageBoxIcon);
    Engine& GetEngine() noexcept;
    const Engine& GetEngine() const noexcept;
    std::shared_ptr<Window> GetRootWindow() noexcept;
    std::shared_ptr<const Window> GetRootWindow() const noexcept;
    
protected:
    std::unique_ptr<Engine> _engine;
    std::shared_ptr<Window> _rootWindow;
};

}
