#include "Core/Algorithm.h"

#include "Keyboard.h"

TG_NAMESPACE_BEGIN
    
Keyboard::Keyboard(gainput::InputDeviceKeyboard* nativeKeyboard) noexcept :
    _nativeKeyboard(nativeKeyboard)
{
}

void Keyboard::Update()
{
}
    
bool Keyboard::IsKeyDown(KeyCode keyCode) const
{
    const auto castedKeyCode = UnderlyingCast(keyCode);
    return _nativeKeyboard->GetBoolPrevious(castedKeyCode) == false && _nativeKeyboard->GetBool(castedKeyCode);
}
    
bool Keyboard::IsKeyHold(KeyCode keyCode) const
{
    const auto castedKeyCode = UnderlyingCast(keyCode);
    return _nativeKeyboard->GetBoolPrevious(castedKeyCode) && _nativeKeyboard->GetBool(castedKeyCode);
}
    
bool Keyboard::IsKeyUp(KeyCode keyCode) const
{
    const auto castedKeyCode = UnderlyingCast(keyCode);
    return _nativeKeyboard->GetBoolPrevious(castedKeyCode) && _nativeKeyboard->GetBool(castedKeyCode) == false;
}
    
}
