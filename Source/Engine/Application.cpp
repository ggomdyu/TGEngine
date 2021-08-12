#include "PrecompiledHeader.h"

#include "Application.h"

namespace tg
{

Application& Application::GetInstance() noexcept
{
    static Application application;
    return application;
}

PlatformApplication& Application::GetPlatformDependency() noexcept
{
    return *this;
}

const PlatformApplication& Application::GetPlatformDependency() const noexcept
{
    return *this;
}

void Application::Initialize(std::unique_ptr<Engine> engine)
{
    m_engine = std::move(engine);
    m_rootWindow = std::make_unique<Window>(m_engine->GetEngineConfiguration().windowStyle);

    m_engine->Initialize();
}

void Application::Terminate()
{
    std::terminate();
}

void Application::ShowMessageBox(const char8_t* message)
{
    ShowMessageBox(u8"", message);
}

void Application::ShowMessageBox(const char8_t* message, MessageBoxIcon messageBoxIcon)
{
    ShowMessageBox(u8"", message, messageBoxIcon);
}

void Application::ShowMessageBox(const char8_t* title, const char8_t* message)
{
    ShowMessageBox(title, message, MessageBoxIcon::Informational);
}

Engine& Application::GetEngine() noexcept
{
    return *m_engine.get();
}

const Engine& Application::GetEngine() const noexcept
{
    return *m_engine.get();
}

std::shared_ptr<Window> Application::GetRootWindow() noexcept
{
    return m_rootWindow;
}

std::shared_ptr<const Window> Application::GetRootWindow() const noexcept
{
    return m_rootWindow;
}

}
