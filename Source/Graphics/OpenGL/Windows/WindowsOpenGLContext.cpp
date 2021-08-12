#include "PrecompiledHeader.h"

#include <GL/glew.h>
#include <GL/wglew.h>
#include <cassert>

#include "Platform/Windows/Windows.h"
#include "Diagnostics/Debug.h"

#include "../OpenGLContext.h"

namespace tg
{
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
    m_wndHandle(static_cast<HWND>(nativeWindow)),
    m_dcHandle(GetDC(m_wndHandle))
{
    HGLRC oldGLRC = MakeOldGLRC(m_dcHandle, videoMode.colorBits, videoMode.depthBits,videoMode.stencilBits);
    if (oldGLRC == nullptr)
    {
        return;
    }

    if (wglMakeCurrent(m_dcHandle, oldGLRC) == FALSE)
    {
        return;
    }
 
    if (glewInit() != GLEW_OK)
    {
        return;
    }

    // Create a new version of GLRC.
    m_context = MakeNewGLRC(m_dcHandle, videoMode.colorBits, videoMode.depthBits, videoMode.stencilBits, 4, 1);
    if (m_context == nullptr)
    {
        return;
    }

    // The old OpenGL context is attached to the new GLRC. thus, release it.
    wglMakeCurrent(nullptr, nullptr);
    wglDeleteContext(oldGLRC);
    if (::wglMakeCurrent(m_dcHandle, m_context) == FALSE)
    {
        return;
    }
}

OpenGLContext::OpenGLContext(OpenGLContext&& rhs) noexcept :
    m_wndHandle(rhs.m_wndHandle),
    m_context(rhs.m_context),
    m_dcHandle(rhs.m_dcHandle)
{
    rhs.m_wndHandle = nullptr;
    rhs.m_context = nullptr;
    rhs.m_dcHandle = nullptr;
}

OpenGLContext::~OpenGLContext()
{
    if (m_context != nullptr)
    {
        wglMakeCurrent(nullptr, nullptr);
        wglDeleteContext(m_context);
    }

    if (m_wndHandle != nullptr && m_dcHandle != nullptr)
    {
        ReleaseDC(m_wndHandle, m_dcHandle);
    }

    m_context = nullptr;
    m_dcHandle = nullptr;
}

OpenGLContext& OpenGLContext::operator=(OpenGLContext&& rhs) noexcept
{
    std::swap(m_wndHandle, rhs.m_wndHandle);
    std::swap(m_context, rhs.m_context);
    std::swap(m_dcHandle, rhs.m_dcHandle);

    return *this;
}

void OpenGLContext::MakeCurrent()
{
    wglMakeCurrent(m_dcHandle, m_context);
}

void OpenGLContext::SwapBuffer()
{
    SwapBuffers(m_dcHandle);
}

}