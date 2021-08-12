#pragma once

#include <gainput/gainput.h>

#include "Core/NonCopyable.h"
#include "Math/Vector2.h"

namespace tg
{

enum class MouseCode
{
    Mouse0      = gainput::MouseButton0,
    Mouse1      = gainput::MouseButton1,
    Mouse2      = gainput::MouseButton2,
    Mouse3      = gainput::MouseButton3,
    Mouse4      = gainput::MouseButton4,
    Mouse5      = gainput::MouseButton5,
    Mouse6      = gainput::MouseButton6,
    Mouse7      = gainput::MouseButton7,
    Mouse8      = gainput::MouseButton8,
    Mouse9      = gainput::MouseButton9,
    Mouse10     = gainput::MouseButton10,
    Mouse11     = gainput::MouseButton11,
    Mouse12     = gainput::MouseButton12,
    Mouse13     = gainput::MouseButton13,
    Mouse14     = gainput::MouseButton14,
    Mouse15     = gainput::MouseButton15,
    Mouse16     = gainput::MouseButton16,
    Mouse17     = gainput::MouseButton17,
    Mouse18     = gainput::MouseButton18,
    Mouse19     = gainput::MouseButton19,
    Mouse20     = gainput::MouseButton20,
    Left        = Mouse0,
    Middle      = Mouse1,
    Right       = Mouse2,
    WheelUp     = Mouse3,
    WheelDown   = Mouse4,
};

class Mouse final :
    private NonCopyable
{
/**@section Constructor */
public:
    explicit Mouse(gainput::InputDeviceMouse* nativeMouse) noexcept;

/**@section Method */
public:
    void Update();
    static void GetPosition(int32_t* x, int32_t* y);
    static IntVector2 GetPosition();
    [[nodiscard]] bool IsMouseDown(MouseCode mouseCode) const;
    [[nodiscard]] bool IsMouseHold(MouseCode mouseCode) const;
    [[nodiscard]] bool IsMouseUp(MouseCode mouseCode) const;

/**@section Variable */
private:
    gainput::InputDeviceMouse* m_nativeMouse;
};
    
}
