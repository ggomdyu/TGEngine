#pragma once

#if TGON_GRAPHICS_OPENGL
#include <GL/glew.h>

namespace tg
{

class OpenGLShaderProgram
{
/**@section Constructor */
protected:
    explicit OpenGLShaderProgram(GLuint programId) noexcept;

public:
    OpenGLShaderProgram(const OpenGLShaderProgram& rhs) = delete;
    OpenGLShaderProgram(OpenGLShaderProgram&& rhs) noexcept;

/**@section Destructor */
public:
    ~OpenGLShaderProgram();
    
/**@section Operator */
public:
    OpenGLShaderProgram& operator=(const OpenGLShaderProgram& rhs) = delete;
    OpenGLShaderProgram& operator=(OpenGLShaderProgram&& rhs) noexcept;
    
/**@section Method */
public:
    [[nodiscard]] GLuint GetProgramId() const noexcept;
    
/**@section Variable */
protected:
    GLuint m_programId;
    inline static OpenGLShaderProgram* m_latelyUsedShaderProgram;
};
    
using PlatformShaderProgram = OpenGLShaderProgram;
    
}
#endif
