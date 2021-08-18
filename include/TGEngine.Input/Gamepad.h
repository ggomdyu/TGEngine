#pragma once

#include <gainput/gainput.h>

TG_NAMESPACE_BEGIN

class Gamepad final
{
public:
    explicit Gamepad(gainput::InputDevicePad* nativeGamePad) noexcept;
    
public:
    void Vibrate(float leftMotor, float rightMotor);
    void Update();
    bool IsButtonDown(int32_t buttonNumber) const;
    bool IsButtonHold(int32_t buttonNumber) const;
    bool IsButtonUp(int32_t buttonNumber) const;

private:
    gainput::InputDevicePad* _nativeGamepad;
};
    
TG_NAMESPACE_END