#pragma once

#if TGON_GRAPHICS_OPENGL
#include <GL/glew.h>

#include "Core/NonCopyable.h"
#include "Math/Extent.h"

namespace tg
{
   
class OpenGLRenderTarget :
    private NonCopyable
{
/**@section Constructor */
protected:
    OpenGLRenderTarget(const FExtent2D& extent, int32_t depthBits, int32_t stencilBits);
    OpenGLRenderTarget(OpenGLRenderTarget&& rhs) noexcept;
    
/**@section Destructor */
protected:
    ~OpenGLRenderTarget();

/**@section Operator */
protected:
    OpenGLRenderTarget& operator=(OpenGLRenderTarget&& rhs) noexcept;
    
/**@section Method */
protected:
    static GLuint CreateColorBuffer(const FExtent2D& extent);
    static GLuint CreateDepthBuffer(const FExtent2D& extent, int32_t depthBits);
    static GLuint CreateDepthStencilBuffer(const FExtent2D& extent, int32_t depthBits, int32_t stencilBits);
    static GLuint CreateFrameBuffer(GLuint colorBufferHandle, GLuint depthStencilBufferHandle);
    void Destroy();

/**@section Variable */
protected:
    FExtent2D m_extent;
    GLuint m_frameBufferHandle = 0;
    GLuint m_colorBufferHandle = 0;
    GLuint m_depthStencilBufferHandle = 0;
};
    
using PlatformRenderTarget = OpenGLRenderTarget;

}
#endif
