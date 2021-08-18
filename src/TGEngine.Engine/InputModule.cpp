#include "InputModule.h"

TG_NAMESPACE_BEGIN

InputModule::InputModule(const InputMode& inputMode) :
    _keyboard(inputMode.isUseKeyboard ? _inputManager.CreateKeyboard() : nullptr),
    _mouse(inputMode.isUseMouse ? _inputManager.CreateMouse() : nullptr)
{
}

bool InputModule::GetKey(KeyCode key) const noexcept
{
    return _keyboard->IsKeyHold(key);
}

bool InputModule::GetKeyDown(KeyCode key) const noexcept
{
    return _keyboard->IsKeyDown(key);
}

bool InputModule::GetKeyUp(KeyCode key) const noexcept
{
    return _keyboard->IsKeyUp(key);
}

bool InputModule::GetMouseButton(int32_t button) const noexcept
{
    constexpr MouseCode mouseCodeTable[] = {
        MouseCode::Left,
        MouseCode::Right,
        MouseCode::Middle
    };

    return _mouse->IsMouseHold(mouseCodeTable[button]);
}

bool InputModule::GetMouseButtonDown(int32_t button) const noexcept
{
    constexpr MouseCode mouseCodeTable[] = {
        MouseCode::Left,
        MouseCode::Right,
        MouseCode::Middle
    };

    return _mouse->IsMouseDown(mouseCodeTable[button]);
}

bool InputModule::GetMouseButtonUp(int32_t button) const noexcept
{
    constexpr MouseCode mouseCodeTable[] = {
        MouseCode::Left,
        MouseCode::Right,
        MouseCode::Middle
    };

    return _mouse->IsMouseUp(mouseCodeTable[button]);
}

void InputModule::Update()
{
    _inputManager.Update();

    if (_keyboard != nullptr)
    {
        _keyboard->Update();
    }

    if (_mouse != nullptr)
    {
        _mouse->Update();
    }
}

}
