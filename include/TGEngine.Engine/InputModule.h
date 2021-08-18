#pragma once

#include <memory>

#include "Input/InputManager.h"

#include "Module.h"

TG_NAMESPACE_BEGIN

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
    TG_OBJECT(InputModule)
    
public:
    explicit InputModule(const InputMode& inputMode = {});

public:
    void Update() override;
    bool GetKey(KeyCode key) const noexcept;
    bool GetKeyDown(KeyCode key) const noexcept;
    bool GetKeyUp(KeyCode key) const noexcept;
    bool GetMouseButton(int32_t button) const noexcept;
    bool GetMouseButtonDown(int32_t button) const noexcept;
    bool GetMouseButtonUp(int32_t button) const noexcept;

public:
    static constexpr auto ModuleStage = ModuleStage::Update;

private:
    InputManager _inputManager;
    std::unique_ptr<Keyboard> _keyboard;
    std::unique_ptr<Mouse> _mouse;
};

}
