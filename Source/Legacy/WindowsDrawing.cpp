#include "PrecompiledHeader.h"

#ifndef NOMINMAX
#    define NOMINMAX
#endif
#include <Windows.h>

#include "../Drawing.h"

namespace tgon
{

void DrawPoint(Window& window, const Point& p)
{
    HWND wndHandle = reinterpret_cast<HWND>(window.GetNativeWindow());
    HDC dcHandle = ::GetDC(wndHandle);
    {
        SetPixel(dcHandle, static_cast<int>(p.x), static_cast<int>(p.y), RGB(0, 0, 0));
    }
    ::ReleaseDC(wndHandle, dcHandle);
}

void DrawLine(Window& window, const Point& p1, const Point& p2)
{
    HWND wndHandle = reinterpret_cast<HWND>(window.GetNativeWindow());
    HDC dcHandle = ::GetDC(wndHandle);
    {
        ::MoveToEx(dcHandle, static_cast<int>(p1.x), static_cast<int>(p1.y), RGB(0, 0, 0));
        ::LineTo(dcHandle, static_cast<int>(p2.x), static_cast<int>(p2.y));
    }
    ::ReleaseDC(wndHandle, dcHandle);
}

void DrawPolygon(Window& window, const Point& p1, const Point& p2, const Point& p3)
{
    const POINT ptTable[3] =
    {
        {static_cast<LONG>(p1.x), static_cast<LONG>(p1.y)},
        {static_cast<LONG>(p2.x), static_cast<LONG>(p2.y)},
        {static_cast<LONG>(p3.x), static_cast<LONG>(p3.y)},
    };

    HWND wndHandle = reinterpret_cast<HWND>(window.GetNativeWindow());
    HDC dcHandle = ::GetDC(wndHandle);
    {
        ::Polygon(dcHandle, ptTable, 3);
    }
    ::ReleaseDC(wndHandle, dcHandle);
}

} /* namespace tgon */