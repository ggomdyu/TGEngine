#pragma once

#if TG_GRAPHICS_OPENGL
#include <GL/glew.h>

#include "Core/NonCopyable.h"

TG_NAMESPACE_BEGIN

class OpenGLTexture :
    private NonCopyable
{
public:
    OpenGLTexture();
    OpenGLTexture(OpenGLTexture&& rhs) noexcept;
    ~OpenGLTexture();

protected:
    OpenGLTexture& operator=(OpenGLTexture&& rhs) noexcept;

protected:
    void Use();
    void Unuse();
    void UpdateTexParameters();
    void CreateMipmap() const;
    void Destroy();

protected:
    GLuint _textureHandle;
};

using PlatformTexture = OpenGLTexture;

}
#endif
