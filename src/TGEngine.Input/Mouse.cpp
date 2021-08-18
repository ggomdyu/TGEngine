#if TG_PLATFORM_WINDOWS
#include <Windows.h>
#elif TG_PLATFORM_MACOS
#include <AppKit/AppKit.h>
#endif

#include "Core/Algorithm.h"
#include "Engine/Application.h"

#include "Mouse.h"

TG_NAMESPACE_BEGIN

Mouse::Mouse(gainput::InputDeviceMouse* nativeMouse) noexcept :
    _nativeMouse(nativeMouse)
{
}

void Mouse::Update()
{
}

IntVector2 Mouse::GetPosition()
{
    IntVector2 pt;
    GetPosition(&pt.x, &pt.y);

    return pt;
}

void Mouse::GetPosition(int32_t* x, int32_t* y)
{
#if TG_PLATFORM_WINDOWS
    POINT pt;
    GetCursorPos(&pt);
    ScreenToClient(static_cast<HWND>(Application::GetInstance().GetRootWindow()->GetNativeWindow()), &pt);

    *x = static_cast<int32_t>(pt.x);
    *y = static_cast<int32_t>(pt.y);
#elif TG_PLATFORM_MACOS
    NSPoint pt = [NSEvent mouseLocation];
    pt.y = [NSScreen mainScreen].frame.size.height - pt.y;
    
    *x = static_cast<int32_t>(pt.x);
    *y = static_cast<int32_t>(pt.y);
#else
    *x = 0;
    *y = 0;
#endif
}

bool Mouse::IsMouseDown(MouseCode mouseCode) const
{
    const auto castedMouseCode = UnderlyingCast(mouseCode);
    return _nativeMouse->GetBoolPrevious(castedMouseCode) == false && _nativeMouse->GetBool(castedMouseCode);
}
    
bool Mouse::IsMouseHold(MouseCode mouseCode) const
{
    const auto castedMouseCode = UnderlyingCast(mouseCode);
    return _nativeMouse->GetBoolPrevious(castedMouseCode) && _nativeMouse->GetBool(castedMouseCode);
}

bool Mouse::IsMouseUp(MouseCode mouseCode) const
{
    const auto castedMouseCode = UnderlyingCast(mouseCode);
    return _nativeMouse->GetBoolPrevious(castedMouseCode) && _nativeMouse->GetBool(castedMouseCode) == false;
}

}
