#include "PrecompiledHeader.h"

#include "Core/Algorithm.h"

#include "Keyboard.h"

namespace tg
{
    
Keyboard::Keyboard(gainput::InputDeviceKeyboard* nativeKeyboard) noexcept :
    m_nativeKeyboard(nativeKeyboard)
{
}

void Keyboard::Update()
{
}
    
bool Keyboard::IsKeyDown(KeyCode keyCode) const
{
    const auto castedKeyCode = UnderlyingCast(keyCode);
    return m_nativeKeyboard->GetBoolPrevious(castedKeyCode) == false && m_nativeKeyboard->GetBool(castedKeyCode);
}
    
bool Keyboard::IsKeyHold(KeyCode keyCode) const
{
    const auto castedKeyCode = UnderlyingCast(keyCode);
    return m_nativeKeyboard->GetBoolPrevious(castedKeyCode) && m_nativeKeyboard->GetBool(castedKeyCode);
}
    
bool Keyboard::IsKeyUp(KeyCode keyCode) const
{
    const auto castedKeyCode = UnderlyingCast(keyCode);
    return m_nativeKeyboard->GetBoolPrevious(castedKeyCode) && m_nativeKeyboard->GetBool(castedKeyCode) == false;
}
    
}
