#pragma once

#if TG_GRAPHICS_OPENGL
#if TG_PLATFORM_WINDOWS
#ifndef WIN32_LEAN_AND_MEAN
#define WIN32_LEAN_AND_MEAN 1
#endif
#include <Platform/Windows/Windows.h>
#elif TG_PLATFORM_MACOS
#include <AppKit/NSOpenGL.h>
#include <OpenGL/OpenGL.h>
#endif

#include "../VideoMode.h"

TG_NAMESPACE_BEGIN

class OpenGLContext final
{
public:
    OpenGLContext(void* nativeWindow, const VideoMode& videoMode);
    OpenGLContext(const OpenGLContext& rhs) = delete;
    OpenGLContext(OpenGLContext&& rhs) noexcept;
    ~OpenGLContext();

public:
    OpenGLContext& operator=(const OpenGLContext& rhs) noexcept = delete;
    OpenGLContext& operator=(OpenGLContext&& rhs) noexcept;

public:
    void MakeCurrent();
    void SwapBuffer();

private:
#if TG_PLATFORM_WINDOWS
    HWND _wndHandle = nullptr;
    HGLRC _context = nullptr;
    HDC _dcHandle = nullptr;
#elif TG_PLATFORM_MACOS
    NSOpenGLPixelFormat* _pixelFormat = nil;
    NSOpenGLContext* _context = nil;
#elif TG_PLATFORM_ANDROID
#elif TG_PLATFORM_IOS
#endif
};

}
#endif
