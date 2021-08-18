#if TG_GRAPHICS_OPENGL
#include "Math/Vector4.h"

#include "OpenGLDebug.h"

#include "../ShaderProgram.h"

TG_NAMESPACE_BEGIN
namespace
{

GLuint CreateShaderProgram(GLuint vertexShaderId, GLuint fragmentShaderId)
{
    GLuint programId = 0;
    TG_GL_ERROR_CHECK(programId = glCreateProgram());
    if (programId == 0)
    {
        return 0;
    }

    TG_GL_ERROR_CHECK(glAttachShader(programId, vertexShaderId));
    TG_GL_ERROR_CHECK(glAttachShader(programId, fragmentShaderId));
    TG_GL_ERROR_CHECK(glLinkProgram(programId));

    TG_GL_ERROR_CHECK(glDetachShader(programId, vertexShaderId));
    TG_GL_ERROR_CHECK(glDetachShader(programId, fragmentShaderId));

    TG_GL_ERROR_CHECK(glDeleteShader(vertexShaderId));
    TG_GL_ERROR_CHECK(glDeleteShader(fragmentShaderId));
    
    return programId;
}

std::u8string GetShaderLog(GLuint shaderId)
{
    GLint infoLogLen;
    TG_GL_ERROR_CHECK(glGetShaderiv(shaderId, GL_INFO_LOG_LENGTH, &infoLogLen));

    std::u8string infoLog;
    infoLog.resize(static_cast<size_t>(infoLogLen) + 1);
    TG_GL_ERROR_CHECK(glGetShaderInfoLog(shaderId, infoLogLen, nullptr, reinterpret_cast<char*>(&infoLog[0])));

    return infoLog;
}

bool IsShaderCompileSucceed(GLuint shaderId)
{
    GLint shaderCompileStatus;
    TG_GL_ERROR_CHECK(glGetShaderiv(shaderId, GL_COMPILE_STATUS, &shaderCompileStatus));

    return shaderCompileStatus == GL_TRUE;
}

GLuint CompileShader(GLenum shaderType, const char* shaderCode)
{
    GLuint shaderId = 0;
    TG_GL_ERROR_CHECK(shaderId = glCreateShader(shaderType));
    TG_GL_ERROR_CHECK(glShaderSource(shaderId, 1, &shaderCode, nullptr));

    // Compile the shader code that stored in the shader object.
    TG_GL_ERROR_CHECK(glCompileShader(shaderId));
    if (IsShaderCompileSucceed(shaderId) == false)
    {
        Debug::WriteLine(fmt::format(u8"Failed to invoke glCompileShader. ({0})", GetShaderLog(shaderId)));
        return 0;
    }

    return shaderId;
}

}

OpenGLShaderProgram::OpenGLShaderProgram(GLuint programId) noexcept :
    _programId(programId)
{
}

OpenGLShaderProgram::OpenGLShaderProgram(OpenGLShaderProgram&& rhs) noexcept :
    _programId(rhs._programId)
{
    rhs._programId = 0;
}

OpenGLShaderProgram::~OpenGLShaderProgram()
{
    TG_GL_ERROR_CHECK(glDeleteProgram(_programId));
}
    
OpenGLShaderProgram& OpenGLShaderProgram::operator=(OpenGLShaderProgram&& rhs) noexcept
{
    std::swap(_programId, rhs._programId);
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
    return _programId;
}

void ShaderProgram::Use()
{
    if (_latelyUsedShaderProgram == this)
    {
        return;
    }
    
    this->ResetUniformCache();
    TG_GL_ERROR_CHECK(glUseProgram(_programId));
    
    _latelyUsedShaderProgram = this;
}

void ShaderProgram::Disuse()
{
    TG_GL_ERROR_CHECK(glUseProgram(0));
    
    _latelyUsedShaderProgram = nullptr;
}

void ShaderProgram::BindAttributeLocation(const char* name, uint32_t index)
{
    TG_GL_ERROR_CHECK(glBindAttribLocation(_programId, index, name));
}

int32_t ShaderProgram::GetUniformLocation(const char* name) const
{
    GLint location = 0;
    TG_GL_ERROR_CHECK(location = glGetUniformLocation(_programId, name));

    return static_cast<int32_t>(location);
}

void ShaderProgram::SetParameter1f(int32_t location, float f)
{
    auto& uniformCache = _uniformCache[location].x;
    if (uniformCache == f)
    {
        return;
    }
    
    uniformCache = f;
    
    TG_GL_ERROR_CHECK(glProgramUniform1f(_programId, location, f));
}

void ShaderProgram::SetParameter2f(int32_t location, float f1, float f2)
{
    auto* const uniformCache = reinterpret_cast<Vector2*>(&_uniformCache[location].x);
    const auto v = Vector2(f1, f2);
    if (*uniformCache == v)
    {
        return;
    }
    
    *uniformCache = v;
    
    TG_GL_ERROR_CHECK(glProgramUniform2f(_programId, location, f1, f2));
}

void ShaderProgram::SetParameter3f(int32_t location, float f1, float f2, float f3)
{
    auto* const uniformCache = reinterpret_cast<Vector3*>(&_uniformCache[location].x);
    const auto v = Vector3(f1, f2, f3);
    if (*uniformCache == v)
    {
        return;
    }
    
    *uniformCache = v;
    
    TG_GL_ERROR_CHECK(glProgramUniform3f(_programId, location, f1, f2, f3));
}

void ShaderProgram::SetParameter4f(int32_t location, float f1, float f2, float f3, float f4)
{
    auto* const uniformCache = reinterpret_cast<Vector4*>(&_uniformCache[location].x);
    const auto v = Vector4(f1, f2, f3, f4);
    if (*uniformCache == v)
    {
        return;
    }
    
    *uniformCache = v;
    
    TG_GL_ERROR_CHECK(glProgramUniform4f(_programId, location, f1, f2, f3, f4));
}

void ShaderProgram::SetParameterMatrix4fv(int32_t location, const float* f)
{
    TG_GL_ERROR_CHECK(glProgramUniformMatrix4fv(_programId, location, 1, GL_FALSE, f));
}

void ShaderProgram::SetParameterSampler(int32_t location, uint32_t textureUnit, uint32_t texture)
{
    TG_GL_ERROR_CHECK(glActiveTexture(GL_TEXTURE0 + textureUnit));

    TG_GL_ERROR_CHECK(glBindTexture(GL_TEXTURE_2D, texture));
    TG_GL_ERROR_CHECK(glProgramUniform1i(_programId, location, texture));
}

}
#endif
