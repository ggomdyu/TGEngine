#pragma once

#include <memory>

#include "Keyboard.h"
#include "Mouse.h"
#include "Gamepad.h"

namespace tg
{

class InputManager final
{
/**@section Constructor */
public:
    InputManager();
    InputManager(const InputManager& rhs) = delete;
    InputManager(InputManager&& rhs) = default;

/**@section Destructor */
public:
    ~InputManager();

/**@section Operator */
public:
    InputManager& operator=(const InputManager& rhs) = delete;
    InputManager& operator=(InputManager&& rhs) = default;

/**@section Method */
public:
    void Update();
    [[nodiscard]] std::unique_ptr<Keyboard> CreateKeyboard() const;
    [[nodiscard]] std::unique_ptr<Mouse> CreateMouse() const;
    [[nodiscard]] std::unique_ptr<Gamepad> CreateGamePad() const;

/**@section Variable */
private:
    inline static std::unique_ptr<gainput::InputManager> m_inputManager;
};

}
