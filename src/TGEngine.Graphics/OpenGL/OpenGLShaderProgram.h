#pragma once

#if TG_GRAPHICS_OPENGL
#include <GL/glew.h>

TG_NAMESPACE_BEGIN

class OpenGLShaderProgram
{
protected:
    explicit OpenGLShaderProgram(GLuint programId) noexcept;

public:
    OpenGLShaderProgram(const OpenGLShaderProgram& rhs) = delete;
    OpenGLShaderProgram(OpenGLShaderProgram&& rhs) noexcept;
    ~OpenGLShaderProgram();
    
public:
    OpenGLShaderProgram& operator=(const OpenGLShaderProgram& rhs) = delete;
    OpenGLShaderProgram& operator=(OpenGLShaderProgram&& rhs) noexcept;
    
public:
    GLuint GetProgramId() const noexcept;
    
protected:
    GLuint _programId;
    inline static OpenGLShaderProgram* _latelyUsedShaderProgram;
};
    
using PlatformShaderProgram = OpenGLShaderProgram;
    
}
#endif
