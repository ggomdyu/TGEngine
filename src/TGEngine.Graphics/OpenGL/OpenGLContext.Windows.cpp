#include <GL/glew.h>
#include <GL/wglew.h>
#include <cassert>

#include "Platform/Windows/Windows.h"
#include "Diagnostics/Debug.h"

#include "../OpenGLContext.h"

TG_NAMESPACE_BEGIN
namespace
{

HGLRC MakeOldGLRC(HDC dcHandle, int colorBits, int depthBits, int stencilBits)
{
    PIXELFORMATDESCRIPTOR pixelFormatDesc =
    {
        .nSize = sizeof(decltype(pixelFormatDesc)),
        .nVersion = 1,
        .dwFlags = PFD_DRAW_TO_WINDOW | PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER,
        .iPixelType = PFD_TYPE_RGBA,
        .cColorBits = BYTE(colorBits),
        .cDepthBits = BYTE(depthBits),
        .cStencilBits = BYTE(stencilBits),
        .iLayerType = PFD_MAIN_PLANE,
    };

    const auto pixelFormat = ChoosePixelFormat(dcHandle, &pixelFormatDesc);
    if (pixelFormat == 0)
    {
        return nullptr;
    }

    if (SetPixelFormat(dcHandle, pixelFormat, &pixelFormatDesc) == FALSE)
    {
        return nullptr;
    }

    return wglCreateContext(dcHandle);
}

HGLRC MakeNewGLRC(HDC dcHandle, int colorBits, int depthBits, int stencilBits, int majorVersion, int minorVersion)
{
    int pixelFormatAttributes[] =
    {
        WGL_DRAW_TO_WINDOW_ARB, /*=*/ GL_TRUE,
        WGL_DOUBLE_BUFFER_ARB,  /*=*/ GL_TRUE,
        WGL_PIXEL_TYPE_ARB,     /*=*/ WGL_TYPE_RGBA_ARB,
        WGL_COLOR_BITS_ARB,     /*=*/ colorBits,
        WGL_DEPTH_BITS_ARB,     /*=*/ depthBits,
        WGL_STENCIL_BITS_ARB,   /*=*/ stencilBits,
        0
    };
    int pixelFormat = 0;
    UINT formatCount = 0;
    if (wglChoosePixelFormatARB(dcHandle, pixelFormatAttributes, nullptr, 1, &pixelFormat, &formatCount) == FALSE)
    {
        return nullptr;
    }

    PIXELFORMATDESCRIPTOR pfd {};
    if (SetPixelFormat(dcHandle, pixelFormat, &pfd) == FALSE)
    {
        return nullptr;
    }

    int contextAttributes[64] =
    {
        WGL_CONTEXT_MAJOR_VERSION_ARB, /*=*/ majorVersion,
        WGL_CONTEXT_MINOR_VERSION_ARB, /*=*/ minorVersion,
        WGL_CONTEXT_PROFILE_MASK_ARB,  /*=*/ WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
        0
    };
    return wglCreateContextAttribsARB(dcHandle, nullptr, contextAttributes);
}

}

OpenGLContext::OpenGLContext(void* nativeWindow, const VideoMode& videoMode) :
    _wndHandle(static_cast<HWND>(nativeWindow)),
    _dcHandle(GetDC(_wndHandle))
{
    HGLRC oldGLRC = MakeOldGLRC(_dcHandle, videoMode.colorBits, videoMode.depthBits,videoMode.stencilBits);
    if (oldGLRC == nullptr)
    {
        return;
    }

    if (wglMakeCurrent(_dcHandle, oldGLRC) == FALSE)
    {
        return;
    }
 
    if (glewInit() != GLEW_OK)
    {
        return;
    }

    // Create a new version of GLRC.
    _context = MakeNewGLRC(_dcHandle, videoMode.colorBits, videoMode.depthBits, videoMode.stencilBits, 4, 1);
    if (_context == nullptr)
    {
        return;
    }

    // The old OpenGL context is attached to the new GLRC. thus, release it.
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(oldGLRC);
    if (::wglMakeCurrent(_dcHandle, _context) == FALSE)
    {
        return;
    }
}

OpenGLContext::OpenGLContext(OpenGLContext&& rhs) noexcept :
    _wndHandle(rhs._wndHandle),
    _context(rhs._context),
    _dcHandle(rhs._dcHandle)
{
    rhs._wndHandle = nullptr;
    rhs._context = nullptr;
    rhs._dcHandle = nullptr;
}

OpenGLContext::~OpenGLContext()
{
    if (_context != nullptr)
    {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(_context);
    }

    if (_wndHandle != nullptr && _dcHandle != nullptr)
    {
        ReleaseDC(_wndHandle, _dcHandle);
    }

    _context = nullptr;
    _dcHandle = nullptr;
}

OpenGLContext& OpenGLContext::operator=(OpenGLContext&& rhs) noexcept
{
    std::swap(_wndHandle, rhs._wndHandle);
    std::swap(_context, rhs._context);
    std::swap(_dcHandle, rhs._dcHandle);

    return *this;
}

void OpenGLContext::MakeCurrent()
{
    wglMakeCurrent(_dcHandle, _context);
}

void OpenGLContext::SwapBuffer()
{
    SwapBuffers(_dcHandle);
}

}