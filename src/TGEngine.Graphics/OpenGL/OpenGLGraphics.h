#pragma once

#include <GL/glew.h>

#include "OpenGLContext.h"

TG_NAMESPACE_BEGIN
    
class OpenGLGraphics
{
public:
    OpenGLGraphics(void* nativeWindow, const VideoMode& videoMode);
    OpenGLGraphics(const OpenGLGraphics& rhs) = delete;
    OpenGLGraphics(OpenGLGraphics&& rhs) noexcept;
    ~OpenGLGraphics();

public:
    OpenGLGraphics& operator=(const OpenGLGraphics& rhs) = delete;
    OpenGLGraphics& operator=(OpenGLGraphics&& rhs) noexcept;

protected:
    OpenGLContext _context;
    GLuint _vertexArrayHandle = 0;
};

using PlatformGraphics = OpenGLGraphics;

}
