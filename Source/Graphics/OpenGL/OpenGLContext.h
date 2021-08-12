#pragma once

#if TGON_GRAPHICS_OPENGL
#if TGON_PLATFORM_WINDOWS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <Platform/Windows/Windows.h>
#elif TGON_PLATFORM_MACOS
#include <AppKit/NSOpenGL.h>
#include <OpenGL/OpenGL.h>
#endif

#include "../VideoMode.h"

namespace tg
{

class OpenGLContext final
{
/**@section Constructor */
public:
    OpenGLContext(void* nativeWindow, const VideoMode& videoMode);
    OpenGLContext(const OpenGLContext& rhs) = delete;
    OpenGLContext(OpenGLContext&& rhs) noexcept;

/**@section Destructor */
public:
    ~OpenGLContext();

/**@section Operator */
public:
    OpenGLContext& operator=(const OpenGLContext& rhs) noexcept = delete;
    OpenGLContext& operator=(OpenGLContext&& rhs) noexcept;

/**@section Method */
public:
    void MakeCurrent();
    void SwapBuffer();

/**@section Variable */
private:
#if TGON_PLATFORM_WINDOWS
    HWND m_wndHandle = nullptr;
    HGLRC m_context = nullptr;
    HDC m_dcHandle = nullptr;
#elif TGON_PLATFORM_MACOS
    NSOpenGLPixelFormat* m_pixelFormat = nil;
    NSOpenGLContext* m_context = nil;
#elif TGON_PLATFORM_ANDROID
#elif TGON_PLATFORM_IOS
#endif
};

}
#endif
