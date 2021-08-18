#include <array>
#include <GL/glew.h>
#include <AppKit/NSOpenGLView.h>
#include <AppKit/NSWindow.h>

#include "Platform/Window.h"

#include "../OpenGLContext.h"

TG_NAMESPACE_BEGIN
namespace
{

std::array<NSOpenGLPixelFormatAttribute, 64> QueryGLPixelFormatAttribute(const VideoMode& videoMode)
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

NSOpenGLPixelFormat* FindSuitablePixelFormat(const VideoMode& videoMode)
{
    const auto glPixelFormatAttributes = QueryGLPixelFormatAttribute(videoMode);
    return [[NSOpenGLPixelFormat alloc] initWithAttributes:glPixelFormatAttributes.data()];
}

}

OpenGLContext::OpenGLContext(void* nativeWindow, const VideoMode& videoMode) :
    _context([[NSOpenGLContext alloc] initWithFormat:FindSuitablePixelFormat(videoMode) shareContext:nil])
{
    // Create a view and attach it to the target window.
    auto* openGLView = [[NSOpenGLView alloc] init];
    [openGLView setOpenGLContext: _context];
    [_context makeCurrentContext];
    
    [(__bridge NSWindow*)nativeWindow setContentView:openGLView];
    
    glewInit();
}

OpenGLContext::OpenGLContext(OpenGLContext&& rhs) noexcept :
    _pixelFormat(rhs._pixelFormat),
    _context(rhs._context)
{
    rhs._pixelFormat = nil;
    rhs._context = nil;
}

OpenGLContext::~OpenGLContext()
{
    _pixelFormat = nil;
    _context = nil;
}

OpenGLContext& OpenGLContext::operator=(OpenGLContext&& rhs) noexcept
{
    std::swap(_pixelFormat, rhs._pixelFormat);
    std::swap(_context, rhs._context);
    
    return *this;
}

void OpenGLContext::MakeCurrent()
{
    [_context makeCurrentContext];
}

void OpenGLContext::SwapBuffer()
{
    [_context flushBuffer];
}

}
