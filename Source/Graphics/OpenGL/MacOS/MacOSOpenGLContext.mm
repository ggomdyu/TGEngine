#include "PrecompiledHeader.h"

#include <array>
#include <GL/glew.h>
#include <AppKit/NSOpenGLView.h>
#include <AppKit/NSWindow.h>

#include "Platform/Window.h"

#include "../OpenGLContext.h"

namespace tg
{
namespace
{

[[nodiscard]] std::array<NSOpenGLPixelFormatAttribute, 64> QueryGLPixelFormatAttribute(const VideoMode& videoMode)
{
    std::array<NSOpenGLPixelFormatAttribute, 64> ret{};
    size_t i = 0;

    if (videoMode.enableHardwareAccelerate)
    {
        ret[i++] = NSOpenGLPFAAccelerated;
    }

    ret[i++] = NSOpenGLPFADoubleBuffer;

    if (videoMode.enableMultiSampling)
    {
        ret[i++] = NSOpenGLPFASampleBuffers;
        ret[i++] = 1;
        ret[i++] = NSOpenGLPFASamples;
        ret[i++] = videoMode.sampleCount;
    }

    ret[i++] = NSOpenGLPFAColorSize;
    ret[i++] = 32;
    ret[i++] = NSOpenGLPFADepthSize;
    ret[i++] = 24;
    ret[i++] = NSOpenGLPFAAlphaSize;
    ret[i++] = 8;
    ret[i++] = NSOpenGLPFAOpenGLProfile;
    ret[i++] = NSOpenGLProfileVersion3_2Core;

    // Mark the end of attributes.
    ret[i++] = 0;
}

[[nodiscard]] NSOpenGLPixelFormat* FindSuitablePixelFormat(const VideoMode& videoMode)
{
    const auto glPixelFormatAttributes = QueryGLPixelFormatAttribute(videoMode);
    return [[NSOpenGLPixelFormat alloc] initWithAttributes:glPixelFormatAttributes.data()];
}

}

OpenGLContext::OpenGLContext(void* nativeWindow, const VideoMode& videoMode) :
    m_context([[NSOpenGLContext alloc] initWithFormat:FindSuitablePixelFormat(videoMode) shareContext:nil])
{
    // Create a view and attach it to the target window.
    auto* openGLView = [[NSOpenGLView alloc] init];
    [openGLView setOpenGLContext: m_context];
    [m_context makeCurrentContext];
    
    [(__bridge NSWindow*)nativeWindow setContentView:openGLView];
    
    glewInit();
}

OpenGLContext::OpenGLContext(OpenGLContext&& rhs) noexcept :
    m_pixelFormat(rhs.m_pixelFormat),
    m_context(rhs.m_context)
{
    rhs.m_pixelFormat = nil;
    rhs.m_context = nil;
}

OpenGLContext::~OpenGLContext()
{
    m_pixelFormat = nil;
    m_context = nil;
}

OpenGLContext& OpenGLContext::operator=(OpenGLContext&& rhs) noexcept
{
    std::swap(m_pixelFormat, rhs.m_pixelFormat);
    std::swap(m_context, rhs.m_context);
    
    return *this;
}

void OpenGLContext::MakeCurrent()
{
    [m_context makeCurrentContext];
}

void OpenGLContext::SwapBuffer()
{
    [m_context flushBuffer];
}

}
