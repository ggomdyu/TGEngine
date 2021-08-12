#include "PrecompiledHeader.h"

#include "Platform/Environment.h"
#include "Engine/Application.h"

#include "Engine.h"
#include "AssetModule.h"
#include "InputModule.h"
#include "TimerModule.h"
#include "TaskModule.h"
#include "AudioModule.h"
#include "SceneModule.h"
#include "RendererModule.h"

namespace tg
{

Engine::Engine(EngineConfiguration engineConfig) noexcept :
    m_engineConfig(std::move(engineConfig)),
    m_prevFrameTime(Environment::GetTickCount())
{
}

Engine::~Engine()
{
    this->RemoveAllModule();
}

Engine& Engine::GetInstance() noexcept
{
    return Application::GetInstance().GetEngine();
}

void Engine::Initialize()
{
    this->AddModule<TaskModule>();
    this->AddModule<AudioModule>();
    this->AddModule<TimeModule>();
    this->AddModule<TimerModule>();
    this->AddModule<RendererModule>(Application::GetInstance().GetRootWindow()->GetNativeWindow(), m_engineConfig.videoMode);
    this->AddModule<InputModule>(m_engineConfig.inputMode);
    this->AddModule<SceneModule>();
}

void Engine::Update()
{
    this->UpdateModule();
}

void Engine::UpdateModule()
{
    this->UpdateModuleStage(ModuleStage::Update);

    // Ignore FPS limitation if V-Sync enabled.
    if (m_engineConfig.videoMode.enableVerticalSync == false)
    {
        const auto currentFrameTime = Environment::GetTickCount();
        m_frameTime += static_cast<float>(currentFrameTime - m_prevFrameTime) * 0.001f;
        m_prevFrameTime = currentFrameTime;

        if (m_frameTime > m_targetSecondPerFrame)
        {
            m_frameTime -= m_targetSecondPerFrame;
            this->UpdateModuleStage(ModuleStage::Render);
        }
    }
    else
    {
        this->UpdateModuleStage(ModuleStage::Render);
    }
}

const EngineConfiguration& Engine::GetEngineConfiguration() const noexcept
{
    return m_engineConfig;
}

void Engine::SetTargetFrameRate(float targetFrameRate)
{
    m_targetSecondPerFrame = 1.0f / targetFrameRate;
}

float Engine::GetTargetFrameRate() const noexcept
{
    return 1.0f / m_targetSecondPerFrame;
}

void Engine::UpdateModuleStage(ModuleStage moduleStage)
{
    for (auto& module : m_moduleStage[UnderlyingCast(moduleStage)])
    {
        if (module != nullptr)
        {
            module->Update();
        }
    }
}

void Engine::RemoveAllModule()
{
    for (auto& modules : m_moduleStage)
    {
        while (modules.empty() == false)
        {
            modules.pop_back();
        }
    }
}

}
