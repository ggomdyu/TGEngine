#pragma once

#include <GL/glew.h>

#include "OpenGLContext.h"

namespace tg
{
    
class OpenGLGraphics
{
/**@section Constructor */
public:
    OpenGLGraphics(void* nativeWindow, const VideoMode& videoMode);
    OpenGLGraphics(const OpenGLGraphics& rhs) = delete;
    OpenGLGraphics(OpenGLGraphics&& rhs) noexcept;

/**@section Destructor */
public:
    ~OpenGLGraphics();

/**@section Operator */
public:
    OpenGLGraphics& operator=(const OpenGLGraphics& rhs) = delete;
    OpenGLGraphics& operator=(OpenGLGraphics&& rhs) noexcept;

/**@section Variable */
protected:
    OpenGLContext m_context;
    GLuint m_vertexArrayHandle = 0;
};

using PlatformGraphics = OpenGLGraphics;

}
