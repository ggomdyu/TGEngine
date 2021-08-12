#include "PrecompiledHeader.h"

#include "Gamepad.h"

namespace tg
{

Gamepad::Gamepad(gainput::InputDevicePad* nativeGamepad) noexcept :
    m_nativeGamepad(nativeGamepad)
{
}

void Gamepad::Update()
{
}
    
void Gamepad::Vibrate(float leftMotor, float rightMotor)
{
    m_nativeGamepad->Vibrate(leftMotor, rightMotor);
}
    
bool Gamepad::IsButtonDown(int32_t buttonNumber) const
{
    return false;
}
    
bool Gamepad::IsButtonHold(int32_t buttonNumber) const
{
    return false;
}
    
bool Gamepad::IsButtonUp(int32_t buttonNumber) const
{
    return false;
}

}
