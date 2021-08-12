#pragma once

#include <array>
#include <vector>

#include "Core/Algorithm.h"
#include "Graphics/VideoMode.h"
#include "Platform/WindowStyle.h"

#include "EngineConfiguration.h"

#if TGON_PLATFORM_WINDOWS
#include "Windows/WindowsEngine.h"
#elif TGON_PLATFORM_MACOS
#include "MacOS/MacOSEngine.h"
#elif TGON_PLATFORM_ANDROID
#include "Android/AndroidEngine.h"
#elif TGON_PLATFORM_IOS
#include "IOS/IOSEngine.h"
#endif

namespace tg
{

class Engine :
    public RuntimeObject
{
public:
    TGON_RTTI(Engine)

/**@section Constructor */
public:
    explicit Engine(EngineConfiguration engineConfig) noexcept;
    Engine(const Engine& rhs) = delete;
    Engine(Engine&& rhs) = delete;

/**@section Destructor */
public:
    ~Engine() override;

/**@section Operator */
public:
    Engine& operator=(const Engine& rhs) = delete;
    Engine& operator=(Engine&& rhs) = delete;

/**@section Method */
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
    template <typename _Module, typename... _Types> requires Modularizable<_Module>
    _Module* AddModule(_Types&&... args);

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

/**@section Variable */
private:
    EngineConfiguration m_engineConfig;
    std::array<std::vector<std::unique_ptr<Module>>, 3> m_moduleStage;
    int64_t m_prevFrameTime = 0;
    float m_frameTime = 0.0f;
    float m_targetSecondPerFrame = -1.0f;
};
    
template <typename _Module, typename... _Types> requires Modularizable<_Module>
_Module* Engine::AddModule(_Types&&... args)
{
    auto module = std::make_unique<_Module>(std::forward<_Types>(args)...);
    auto* rawModule = module.get();
    m_moduleStage[UnderlyingCast(_Module::ModuleStage)].push_back(std::move(module));

    rawModule->Initialize();

    return rawModule;
}

template <typename _Module> requires Modularizable<_Module>
_Module* Engine::FindModule() noexcept
{
    for (auto& modules : m_moduleStage)
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
