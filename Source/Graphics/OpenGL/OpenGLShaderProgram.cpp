#include "PrecompiledHeader.h"

#if TGON_GRAPHICS_OPENGL
#include "Math/Vector4.h"

#include "OpenGLDebug.h"

#include "../ShaderProgram.h"

namespace tg
{
namespace
{

[[nodiscard]] GLuint CreateShaderProgram(GLuint vertexShaderId, GLuint fragmentShaderId)
{
    GLuint programId = 0;
    TGON_GL_ERROR_CHECK(programId = glCreateProgram());
    if (programId == 0)
    {
        return 0;
    }

    TGON_GL_ERROR_CHECK(glAttachShader(programId, vertexShaderId));
    TGON_GL_ERROR_CHECK(glAttachShader(programId, fragmentShaderId));
    TGON_GL_ERROR_CHECK(glLinkProgram(programId));

    TGON_GL_ERROR_CHECK(glDetachShader(programId, vertexShaderId));
    TGON_GL_ERROR_CHECK(glDetachShader(programId, fragmentShaderId));

    TGON_GL_ERROR_CHECK(glDeleteShader(vertexShaderId));
    TGON_GL_ERROR_CHECK(glDeleteShader(fragmentShaderId));
    
    return programId;
}

[[nodiscard]] std::u8string GetShaderLog(GLuint shaderId)
{
    GLint infoLogLen;
    TGON_GL_ERROR_CHECK(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLen));

    std::u8string infoLog;
    infoLog.resize(static_cast<size_t>(infoLogLen) + 1);
    TGON_GL_ERROR_CHECK(glGetShaderInfoLog(shaderId, infoLogLen, nullptr, reinterpret_cast<char*>(&infoLog[0])));

    return infoLog;
}

[[nodiscard]] bool IsShaderCompileSucceed(GLuint shaderId)
{
    GLint shaderCompileStatus;
    TGON_GL_ERROR_CHECK(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderCompileStatus));

    return shaderCompileStatus == GL_TRUE;
}

[[nodiscard]] GLuint CompileShader(GLenum shaderType, const char* shaderCode)
{
    GLuint shaderId = 0;
    TGON_GL_ERROR_CHECK(shaderId = glCreateShader(shaderType));
    TGON_GL_ERROR_CHECK(glShaderSource(shaderId, 1, &shaderCode, nullptr));

    // Compile the shader code that stored in the shader object.
    TGON_GL_ERROR_CHECK(glCompileShader(shaderId));
    if (IsShaderCompileSucceed(shaderId) == false)
    {
        Debug::WriteLine(fmt::format(u8"Failed to invoke glCompileShader. ({0})", GetShaderLog(shaderId)));
        return 0;
    }

    return shaderId;
}

}

OpenGLShaderProgram::OpenGLShaderProgram(GLuint programId) noexcept :
    m_programId(programId)
{
}

OpenGLShaderProgram::OpenGLShaderProgram(OpenGLShaderProgram&& rhs) noexcept :
    m_programId(rhs.m_programId)
{
    rhs.m_programId = 0;
}

OpenGLShaderProgram::~OpenGLShaderProgram()
{
    TGON_GL_ERROR_CHECK(glDeleteProgram(m_programId));
}
    
OpenGLShaderProgram& OpenGLShaderProgram::operator=(OpenGLShaderProgram&& rhs) noexcept
{
    std::swap(m_programId, rhs.m_programId);
    return *this;
}

std::optional<ShaderProgram> ShaderProgram::Create(const char* vertexShaderCode, const char* fragmentShaderCode)
{
    const auto vertexShaderId = CompileShader(GL_VERTEX_SHADER, vertexShaderCode);
    const auto fragmentShaderId = CompileShader(GL_FRAGMENT_SHADER, fragmentShaderCode);
    const auto program = CreateShaderProgram(vertexShaderId, fragmentShaderId);
    if (program == 0)
    {
        return {};
    }

    return ShaderProgram(program);
}

GLuint OpenGLShaderProgram::GetProgramId() const noexcept
{
    return m_programId;
}

void ShaderProgram::Use()
{
    if (m_latelyUsedShaderProgram == this)
    {
        return;
    }
    
    this->ResetUniformCache();
    TGON_GL_ERROR_CHECK(glUseProgram(m_programId));
    
    m_latelyUsedShaderProgram = this;
}

void ShaderProgram::Disuse()
{
    TGON_GL_ERROR_CHECK(glUseProgram(0));
    
    m_latelyUsedShaderProgram = nullptr;
}

void ShaderProgram::BindAttributeLocation(const char* name, uint32_t index)
{
    TGON_GL_ERROR_CHECK(glBindAttribLocation(m_programId, index, name));
}

int32_t ShaderProgram::GetUniformLocation(const char* name) const
{
    GLint location = 0;
    TGON_GL_ERROR_CHECK(location = glGetUniformLocation(m_programId, name));

    return static_cast<int32_t>(location);
}

void ShaderProgram::SetParameter1f(int32_t location, float f)
{
    auto& uniformCache = m_uniformCache[location].x;
    if (uniformCache == f)
    {
        return;
    }
    
    uniformCache = f;
    
    TGON_GL_ERROR_CHECK(glProgramUniform1f(m_programId, location, f));
}

void ShaderProgram::SetParameter2f(int32_t location, float f1, float f2)
{
    auto* const uniformCache = reinterpret_cast<Vector2*>(&m_uniformCache[location].x);
    const auto v = Vector2(f1, f2);
    if (*uniformCache == v)
    {
        return;
    }
    
    *uniformCache = v;
    
    TGON_GL_ERROR_CHECK(glProgramUniform2f(m_programId, location, f1, f2));
}

void ShaderProgram::SetParameter3f(int32_t location, float f1, float f2, float f3)
{
    auto* const uniformCache = reinterpret_cast<Vector3*>(&m_uniformCache[location].x);
    const auto v = Vector3(f1, f2, f3);
    if (*uniformCache == v)
    {
        return;
    }
    
    *uniformCache = v;
    
    TGON_GL_ERROR_CHECK(glProgramUniform3f(m_programId, location, f1, f2, f3));
}

void ShaderProgram::SetParameter4f(int32_t location, float f1, float f2, float f3, float f4)
{
    auto* const uniformCache = reinterpret_cast<Vector4*>(&m_uniformCache[location].x);
    const auto v = Vector4(f1, f2, f3, f4);
    if (*uniformCache == v)
    {
        return;
    }
    
    *uniformCache = v;
    
    TGON_GL_ERROR_CHECK(glProgramUniform4f(m_programId, location, f1, f2, f3, f4));
}

void ShaderProgram::SetParameterMatrix4fv(int32_t location, const float* f)
{
    TGON_GL_ERROR_CHECK(glProgramUniformMatrix4fv(m_programId, location, 1, GL_FALSE, f));
}

void ShaderProgram::SetParameterSampler(int32_t location, uint32_t textureUnit, uint32_t texture)
{
    TGON_GL_ERROR_CHECK(glActiveTexture(GL_TEXTURE0 + textureUnit));

    TGON_GL_ERROR_CHECK(glBindTexture(GL_TEXTURE_2D, texture));
    TGON_GL_ERROR_CHECK(glProgramUniform1i(m_programId, location, texture));
}

}
#endif
