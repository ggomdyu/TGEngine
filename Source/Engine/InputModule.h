#pragma once

#include <memory>

#include "Input/InputManager.h"

#include "Module.h"

namespace tg
{

struct InputMode final
{
    bool isUseKeyboard = true;
    bool isUseMouse = true;
    bool isUseGamepad = false;
};

class InputModule final :
    public Module
{
public:
    TGON_RTTI(InputModule)
    
/**@section Constructor */
public:
    explicit InputModule(const InputMode& inputMode = {});

/**@section Method */
public:
    void Update() override;
    [[nodiscard]] bool GetKey(KeyCode key) const noexcept;
    [[nodiscard]] bool GetKeyDown(KeyCode key) const noexcept;
    [[nodiscard]] bool GetKeyUp(KeyCode key) const noexcept;
    [[nodiscard]] bool GetMouseButton(int32_t button) const noexcept;
    [[nodiscard]] bool GetMouseButtonDown(int32_t button) const noexcept;
    [[nodiscard]] bool GetMouseButtonUp(int32_t button) const noexcept;

/**@section Variable */
public:
    static constexpr auto ModuleStage = ModuleStage::Update;

private:
    InputManager m_inputManager;
    std::unique_ptr<Keyboard> m_keyboard;
    std::unique_ptr<Mouse> m_mouse;
};

}
