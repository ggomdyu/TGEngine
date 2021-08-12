#pragma once

#include <gainput/gainput.h>

#include "Core/NonCopyable.h"

namespace tg
{

class Gamepad final :
    private NonCopyable
{
/**@section Constructor */
public:
    explicit Gamepad(gainput::InputDevicePad* nativeGamePad) noexcept;
    
/**@section Method */
public:
    void Vibrate(float leftMotor, float rightMotor);
    void Update();
    [[nodiscard]] bool IsButtonDown(int32_t buttonNumber) const;
    [[nodiscard]] bool IsButtonHold(int32_t buttonNumber) const;
    [[nodiscard]] bool IsButtonUp(int32_t buttonNumber) const;

/**@section Variable */
private:
    gainput::InputDevicePad* m_nativeGamepad;
};
    
}
