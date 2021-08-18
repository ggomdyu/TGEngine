#include "Gamepad.h"

TG_NAMESPACE_BEGIN

Gamepad::Gamepad(gainput::InputDevicePad* nativeGamepad) noexcept :
    _nativeGamepad(nativeGamepad)
{
}

void Gamepad::Update()
{
}
    
void Gamepad::Vibrate(float leftMotor, float rightMotor)
{
    _nativeGamepad->Vibrate(leftMotor, rightMotor);
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
