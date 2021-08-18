#pragma once

#include <array>
#include <vector>

#include "Core/Algorithm.h"
#include "Graphics/VideoMode.h"
#include "Platform/WindowStyle.h"

#include "EngineConfiguration.h"

#if TG_PLATFORM_WINDOWS
#include "Windows/WindowsEngine.h"
#elif TG_PLATFORM_MACOS
#include "MacOS/MacOSEngine.h"
#elif TG_PLATFORM_ANDROID
#include "Android/AndroidEngine.h"
#elif TG_PLATFORM_IOS
#include "IOS/IOSEngine.h"
#endif

TG_NAMESPACE_BEGIN

class Engine :
    public RuntimeObject
{
public:
    TG_OBJECT(Engine)

public:
    explicit Engine(EngineConfiguration engineConfig) noexcept;
    Engine(const Engine& rhs) = delete;
    Engine(Engine&& rhs) = delete;
    ~Engine() override;

public:
    Engine& operator=(const Engine& rhs) = delete;
    Engine& operator=(Engine&& rhs) = delete;

public:
    /**
     * @brief   Gets the global instance of the object.
     * @return  The global instance of the object.
     */
    static Engine& GetInstance() noexcept;

    /**
     * @brief   Initializes the object.
     */
    virtual void Initialize();

    /**
     * @brief   Updates the frame of the object.
     */
    virtual void Update();

    /**
     * @brief   Adds a module to the object.
     * @param args  Arguments for construct the specified type of module.
     * @return  The added module.
     */
    template <typename _Module, typename... Ts> requires Modularizable<_Module>
    _Module* AddModule(Ts&&... args);

    /**
     * @brief   Finds the module of the specified type.
     * @return  The type of module to retrieve or nullptr.
     */
    template <typename _Module> requires Modularizable<_Module>
    _Module* FindModule() noexcept;

    /**
     * @brief   Finds the module of the specified type.
     * @return  The type of module to retrieve or nullptr.
     */
    template <typename _Module> requires Modularizable<_Module>
    const _Module* FindModule() const noexcept;

    /**
     * @brief   Gets the configuration of the engine.
     * @return  The engine configuration.
     */
    const EngineConfiguration& GetEngineConfiguration() const noexcept;

    /**
     * @brief   Forces the engine to render at a specified frame rate.
     * @param targetFrameRate   The frame rate.
     */
    void SetTargetFrameRate(float targetFrameRate);

    /**
     * @brief   Gets the frame rate of the engine.
     * @return  The frame rate.
     */
    float GetTargetFrameRate() const noexcept;

private:
    void UpdateModule();
    void UpdateModuleStage(ModuleStage moduleStage);
    void RemoveAllModule();

private:
    EngineConfiguration _engineConfig;
    std::array<std::vector<std::unique_ptr<Module>>, 3> _moduleStage;
    int64_t _prevFrameTime = 0;
    float _frameTime = 0.0f;
    float _targetSecondPerFrame = -1.0f;
};
    
template <typename _Module, typename... Ts> requires Modularizable<_Module>
_Module* Engine::AddModule(Ts&&... args)
{
    auto module = std::make_unique<_Module>(std::forward<Ts>(args)...);
    auto* rawModule = module.get();
    _moduleStage[UnderlyingCast(_Module::ModuleStage)].push_back(std::move(module));

    rawModule->Initialize();

    return rawModule;
}

template <typename _Module> requires Modularizable<_Module>
_Module* Engine::FindModule() noexcept
{
    for (auto& modules : _moduleStage)
    {
        for (auto& module : modules)
        {
            if (module->GetRtti() == tg::GetRtti<_Module>())
            {
                return reinterpret_cast<_Module*>(module.get());
            }
        }
    }

    return nullptr;
}

template <typename _Module> requires Modularizable<_Module>
const _Module* Engine::FindModule() const noexcept
{
    return const_cast<Engine*>(this)->FindModule<_Module>();
}

}
