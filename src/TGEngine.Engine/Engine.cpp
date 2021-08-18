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

TG_NAMESPACE_BEGIN

Engine::Engine(EngineConfiguration engineConfig) noexcept :
    _engineConfig(std::move(engineConfig)),
    _prevFrameTime(Environment::GetTickCount())
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
    this->AddModule<RendererModule>(Application::GetInstance().GetRootWindow()->GetNativeWindow(), _engineConfig.videoMode);
    this->AddModule<InputModule>(_engineConfig.inputMode);
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
    if (_engineConfig.videoMode.enableVerticalSync == false)
    {
        const auto currentFrameTime = Environment::GetTickCount();
        _frameTime += static_cast<float>(currentFrameTime - _prevFrameTime) * 0.001f;
        _prevFrameTime = currentFrameTime;

        if (_frameTime > _targetSecondPerFrame)
        {
            _frameTime -= _targetSecondPerFrame;
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
    return _engineConfig;
}

void Engine::SetTargetFrameRate(float targetFrameRate)
{
    _targetSecondPerFrame = 1.0f / targetFrameRate;
}

float Engine::GetTargetFrameRate() const noexcept
{
    return 1.0f / _targetSecondPerFrame;
}

void Engine::UpdateModuleStage(ModuleStage moduleStage)
{
    for (auto& module : _moduleStage[UnderlyingCast(moduleStage)])
    {
        if (module != nullptr)
        {
            module->Update();
        }
    }
}

void Engine::RemoveAllModule()
{
    for (auto& modules : _moduleStage)
    {
        while (modules.empty() == false)
        {
            modules.pop_back();
        }
    }
}

}
