#pragma once

#if TGON_GRAPHICS_OPENGL
#include <GL/glew.h>

namespace tg
{
    
class OpenGLVertexBuffer
{
/**@section Constructor */
public:
    explicit OpenGLVertexBuffer(GLuint vertexBufferHandle) noexcept;
    OpenGLVertexBuffer(const OpenGLVertexBuffer& rhs) = delete;
    OpenGLVertexBuffer(OpenGLVertexBuffer&& rhs) noexcept;
    
/**@section Destructor */
public:
    ~OpenGLVertexBuffer() = default;
    
/**@section Operator */
public:
    OpenGLVertexBuffer& operator=(const OpenGLVertexBuffer& rhs) = delete;
    OpenGLVertexBuffer& operator=(OpenGLVertexBuffer&& rhs) noexcept;

/**@section Method */
public:
    [[nodiscard]] static GLuint CreateVertexBufferId();
    [[nodiscard]] GLuint GetVertexBufferHandle() const noexcept;

/**@section Variable */
protected:
    GLuint m_vertexBufferId;
};

using PlatformVertexBuffer = OpenGLVertexBuffer;

}
#endif
