#pragma once

#if TG_GRAPHICS_OPENGL
#include <GL/glew.h>

TG_NAMESPACE_BEGIN

class OpenGLIndexBuffer
{
public:
    explicit OpenGLIndexBuffer(GLuint indexBufferHandle) noexcept;
    OpenGLIndexBuffer(const OpenGLIndexBuffer& rhs) = delete;
    OpenGLIndexBuffer(OpenGLIndexBuffer&& rhs) noexcept;
    ~OpenGLIndexBuffer() = default;
    
public:
    OpenGLIndexBuffer& operator=(const OpenGLIndexBuffer& rhs) = delete;
    OpenGLIndexBuffer& operator=(OpenGLIndexBuffer&& rhs) noexcept;
    
public:
    static GLuint CreateIndexBufferId();
    GLuint GetIndexBufferHandle() const noexcept;

protected:
    GLuint _indexBufferHandle;
};
    
using PlatformIndexBuffer = OpenGLIndexBuffer;

}
#endif
