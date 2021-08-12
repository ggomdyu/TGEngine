#pragma once

#if TGON_GRAPHICS_OPENGL
#include <GL/glew.h>

#include "Core/NonCopyable.h"

namespace tg
{

class OpenGLTexture :
    private NonCopyable
{
/**@section Constructor */
public:
    OpenGLTexture();
    OpenGLTexture(OpenGLTexture&& rhs) noexcept;
    
/**@section Destructor */
protected:
    ~OpenGLTexture();

/**@section Operator */
protected:
    OpenGLTexture& operator=(OpenGLTexture&& rhs) noexcept;

/**@section Method */
protected:
    void Use();
    void Unuse();
    void UpdateTexParameters();
    void CreateMipmap() const;
    void Destroy();

/**@section Variable */
protected:
    GLuint m_textureHandle;
};

using PlatformTexture = OpenGLTexture;

}
#endif
