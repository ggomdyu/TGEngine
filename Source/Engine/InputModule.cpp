#include "PrecompiledHeader.h"

#include "InputModule.h"

namespace tg
{

InputModule::InputModule(const InputMode& inputMode) :
    m_keyboard(inputMode.isUseKeyboard ? m_inputManager.CreateKeyboard() : nullptr),
    m_mouse(inputMode.isUseMouse ? m_inputManager.CreateMouse() : nullptr)
{
}

bool InputModule::GetKey(KeyCode key) const noexcept
{
    return m_keyboard->IsKeyHold(key);
}

bool InputModule::GetKeyDown(KeyCode key) const noexcept
{
    return m_keyboard->IsKeyDown(key);
}

bool InputModule::GetKeyUp(KeyCode key) const noexcept
{
    return m_keyboard->IsKeyUp(key);
}

bool InputModule::GetMouseButton(int32_t button) const noexcept
{
    constexpr MouseCode mouseCodeTable[] = {
        MouseCode::Left,
        MouseCode::Right,
        MouseCode::Middle
    };

    return m_mouse->IsMouseHold(mouseCodeTable[button]);
}

bool InputModule::GetMouseButtonDown(int32_t button) const noexcept
{
    constexpr MouseCode mouseCodeTable[] = {
        MouseCode::Left,
        MouseCode::Right,
        MouseCode::Middle
    };

    return m_mouse->IsMouseDown(mouseCodeTable[button]);
}

bool InputModule::GetMouseButtonUp(int32_t button) const noexcept
{
    constexpr MouseCode mouseCodeTable[] = {
        MouseCode::Left,
        MouseCode::Right,
        MouseCode::Middle
    };

    return m_mouse->IsMouseUp(mouseCodeTable[button]);
}

void InputModule::Update()
{
    m_inputManager.Update();

    if (m_keyboard != nullptr)
    {
        m_keyboard->Update();
    }

    if (m_mouse != nullptr)
    {
        m_mouse->Update();
    }
}

}
