#pragma once

#include <memory>

#include "Keyboard.h"
#include "Mouse.h"
#include "Gamepad.h"

TG_NAMESPACE_BEGIN

class InputManager final
{
public:
    InputManager();
    InputManager(const InputManager& rhs) = delete;
    InputManager(InputManager&& rhs) = default;
    ~InputManager();

public:
    InputManager& operator=(const InputManager& rhs) = delete;
    InputManager& operator=(InputManager&& rhs) = default;

public:
    std::unique_ptr<Keyboard> CreateKeyboard() const;
    std::unique_ptr<Mouse> CreateMouse() const;
    std::unique_ptr<Gamepad> CreateGamePad() const;
    void Update();

private:
    inline static std::unique_ptr<gainput::InputManager> _inputManager;
};

}
