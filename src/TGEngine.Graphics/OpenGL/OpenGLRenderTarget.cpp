#include "Diagnostics/Debug.h"

#include "../RenderTarget.h"

TG_NAMESPACE_BEGIN

OpenGLRenderTarget::OpenGLRenderTarget(const FExtent2D& extent, int32_t depthBits, int32_t stencilBits) :
    _colorBufferHandle(CreateColorBuffer(extent)),
    _depthStencilBufferHandle(CreateDepthStencilBuffer(extent, depthBits, stencilBits)),
    _frameBufferHandle(CreateFrameBuffer(_colorBufferHandle, _depthStencilBufferHandle))
{
}

OpenGLRenderTarget::OpenGLRenderTarget(OpenGLRenderTarget&& rhs) noexcept :
    _extent(rhs._extent),
    _frameBufferHandle(rhs._frameBufferHandle),
    _colorBufferHandle(rhs._colorBufferHandle),
    _depthStencilBufferHandle(rhs._depthStencilBufferHandle)
{
    rhs._frameBufferHandle = 0;
    rhs._colorBufferHandle = 0;
    rhs._depthStencilBufferHandle = 0;
}

OpenGLRenderTarget::~OpenGLRenderTarget()
{
    this->Destroy();
}
    
OpenGLRenderTarget& OpenGLRenderTarget::operator=(OpenGLRenderTarget&& rhs) noexcept
{
    std::swap(_frameBufferHandle, rhs._frameBufferHandle);
    std::swap(_colorBufferHandle, rhs._colorBufferHandle);
    std::swap(_depthStencilBufferHandle, rhs._depthStencilBufferHandle);

    _extent = rhs._extent;
    
    return *this;
}

GLuint OpenGLRenderTarget::CreateColorBuffer(const FExtent2D& extent)
{
    GLuint colorBufferHandle = 0;
    glGenTextures(1, &colorBufferHandle);
    glBindTexture(GL_TEXTURE_2D, colorBufferHandle);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, static_cast<GLsizei>(extent.width), static_cast<GLsizei>(extent.height), 0, GL_RGB, GL_UNSIGNED_BYTE, nullptr);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
    
    return colorBufferHandle;
}

GLuint OpenGLRenderTarget::CreateDepthBuffer(const FExtent2D& extent, int32_t depthBits)
{
    Debug::Assert(depthBits == 32 || depthBits == 24 || depthBits == 16);

    GLuint depthBufferHandle = 0;
    glGenRenderbuffers(1, &depthBufferHandle);
    glBindRenderbuffer(GL_RENDERBUFFER, depthBufferHandle);
    glRenderbufferStorage(GL_RENDERBUFFER, depthBits == 32 ? GL_DEPTH_COMPONENT32 : depthBits == 24 ? GL_DEPTH_COMPONENT24 : GL_DEPTH_COMPONENT16, static_cast<GLsizei>(extent.width), static_cast<GLsizei>(extent.height));
    //glNamedRenderbufferStorage(depthBufferHandle, depthBits == 32 ? GL_DEPTH_COMPONENT32 : depthBits == 24 ? GL_DEPTH_COMPONENT24 : GL_DEPTH_COMPONENT16, static_cast<GLsizei>(extent.width), static_cast<GLsizei>(extent.height));
    
    return depthBufferHandle;
}

GLuint OpenGLRenderTarget::CreateDepthStencilBuffer(const FExtent2D& extent, int32_t depthBits, int32_t stencilBits)
{
    Debug::Assert((depthBits == 32 || depthBits == 24) && stencilBits == 8);

    GLuint depthStencilBuffer = 0;
    glGenRenderbuffers(1, &depthStencilBuffer);
    glBindRenderbuffer(GL_RENDERBUFFER, depthStencilBuffer);
    glRenderbufferStorage(GL_RENDERBUFFER, depthBits == 32 ? GL_DEPTH32F_STENCIL8 : GL_DEPTH24_STENCIL8, static_cast<GLsizei>(extent.width), static_cast<GLsizei>(extent.height));

    return depthStencilBuffer;
}

GLuint OpenGLRenderTarget::CreateFrameBuffer(GLuint colorBufferHandle, GLuint depthStencilBufferHandle)
{
    GLuint frameBufferHandle;
    glGenFramebuffers(1, &frameBufferHandle);

    glBindFramebuffer(GL_FRAMEBUFFER, frameBufferHandle);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, colorBufferHandle, 0);
    glFramebufferRenderbuffer(GL_FRAMEBUFFER, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, depthStencilBufferHandle);

    //glNamedFramebufferTexture2DEXT(_frameBufferHandle, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, _colorBufferHandle, 0);
    //glNamedFramebufferRenderbuffer(_frameBufferHandle, GL_DEPTH_STENCIL_ATTACHMENT, GL_RENDERBUFFER, _depthStencilBufferHandle);
    //glCheckNamedFramebufferStatus(_frameBufferHandle, GL_FRAMEBUFFER);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    return frameBufferHandle;
}

void OpenGLRenderTarget::Destroy()
{
    if (_colorBufferHandle != 0)
    {
        glDeleteTextures(1, &_colorBufferHandle);
        _colorBufferHandle = 0;
    }
    
    if (_depthStencilBufferHandle != 0)
    {
        glDeleteRenderbuffers(1, &_depthStencilBufferHandle);
        _depthStencilBufferHandle = 0;
    }
    
    if (_frameBufferHandle != 0)
    {
        glDeleteFramebuffers(1, &_frameBufferHandle);
        _frameBufferHandle = 0;
    }
}

void RenderTarget::Use()
{
    glBindFramebuffer(GL_FRAMEBUFFER, _frameBufferHandle);
    glViewport(0, 0, static_cast<GLsizei>(_extent.width), static_cast<GLsizei>(_extent.height));
}

void RenderTarget::Disuse()
{
    glBindFramebuffer(GL_FRAMEBUFFER, 0);
}

}