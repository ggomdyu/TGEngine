#pragma once

#if TGON_GRAPHICS_OPENGL
#include <GL/glew.h>

namespace tg
{

class OpenGLIndexBuffer
{
/**@section Constructor */
public:
    explicit OpenGLIndexBuffer(GLuint indexBufferHandle) noexcept;
    OpenGLIndexBuffer(const OpenGLIndexBuffer& rhs) = delete;
    OpenGLIndexBuffer(OpenGLIndexBuffer&& rhs) noexcept;
    
/**@section Destructor */
public:
    ~OpenGLIndexBuffer() = default;
    
/**@section Operator */
public:
    OpenGLIndexBuffer& operator=(const OpenGLIndexBuffer& rhs) = delete;
    OpenGLIndexBuffer& operator=(OpenGLIndexBuffer&& rhs) noexcept;
    
/**@section Method */
public:
    [[nodiscard]] static GLuint CreateIndexBufferId();
    [[nodiscard]] GLuint GetIndexBufferHandle() const noexcept;

/**@section Variable */
protected:
    GLuint m_indexBufferHandle;
};
    
using PlatformIndexBuffer = OpenGLIndexBuffer;

}
#endif
