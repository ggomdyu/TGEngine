#pragma once

#if TG_GRAPHICS_OPENGL
#include <GL/glew.h>

TG_NAMESPACE_BEGIN
    
class OpenGLVertexBuffer
{
public:
    explicit OpenGLVertexBuffer(GLuint vertexBufferHandle) noexcept;
    OpenGLVertexBuffer(const OpenGLVertexBuffer& rhs) = delete;
    OpenGLVertexBuffer(OpenGLVertexBuffer&& rhs) noexcept;
    ~OpenGLVertexBuffer() = default;
    
public:
    OpenGLVertexBuffer& operator=(const OpenGLVertexBuffer& rhs) = delete;
    OpenGLVertexBuffer& operator=(OpenGLVertexBuffer&& rhs) noexcept;

public:
    static GLuint CreateVertexBufferId();
    GLuint GetVertexBufferHandle() const noexcept;

protected:
    GLuint _vertexBufferId;
};

using PlatformVertexBuffer = OpenGLVertexBuffer;

}
#endif
