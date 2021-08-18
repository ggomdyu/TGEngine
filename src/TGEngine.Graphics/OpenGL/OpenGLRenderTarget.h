#pragma once

#if TG_GRAPHICS_OPENGL
#include <GL/glew.h>

#include "Core/NonCopyable.h"
#include "Math/Extent.h"

TG_NAMESPACE_BEGIN
   
class OpenGLRenderTarget :
    private NonCopyable
{
protected:
    OpenGLRenderTarget(const FExtent2D& extent, int32_t depthBits, int32_t stencilBits);
    OpenGLRenderTarget(OpenGLRenderTarget&& rhs) noexcept;
    ~OpenGLRenderTarget();

protected:
    OpenGLRenderTarget& operator=(OpenGLRenderTarget&& rhs) noexcept;
    
protected:
    static GLuint CreateColorBuffer(const FExtent2D& extent);
    static GLuint CreateDepthBuffer(const FExtent2D& extent, int32_t depthBits);
    static GLuint CreateDepthStencilBuffer(const FExtent2D& extent, int32_t depthBits, int32_t stencilBits);
    static GLuint CreateFrameBuffer(GLuint colorBufferHandle, GLuint depthStencilBufferHandle);
    void Destroy();

protected:
    FExtent2D _extent;
    GLuint _frameBufferHandle = 0;
    GLuint _colorBufferHandle = 0;
    GLuint _depthStencilBufferHandle = 0;
};
    
using PlatformRenderTarget = OpenGLRenderTarget;

}
#endif
