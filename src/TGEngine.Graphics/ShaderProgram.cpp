#include <array>

#include "Core/Algorithm.h"

#include "ShaderProgram.h"

TG_NAMESPACE_BEGIN

PlatformShaderProgram& ShaderProgram::GetPlatformDependency() noexcept
{
    return *this;
}
    
const PlatformShaderProgram& ShaderProgram::GetPlatformDependency() const noexcept
{
    return *this;
}

ShaderProgram::ShaderProgram(ShaderProgram&& rhs) noexcept :
    PlatformShaderProgram(std::move(rhs)),
    _uniformLocationCache(rhs._uniformLocationCache)
{
}

ShaderProgram& ShaderProgram::operator=(ShaderProgram&& rhs) noexcept
{
    _uniformLocationCache = rhs._uniformLocationCache;

    PlatformShaderProgram::operator=(std::move(rhs));

    return *this;
}

void ShaderProgram::SetParameter1f(const char* name, float f)
{
    this->SetParameter1f(this->GetUniformLocation(name), f);
}

void ShaderProgram::SetParameter2f(const char* name, float f1, float f2)
{
    this->SetParameter2f(this->GetUniformLocation(name), f1, f2);
}

void ShaderProgram::SetParameter3f(const char* name, float f1, float f2, float f3)
{
    this->SetParameter3f(this->GetUniformLocation(name), f1, f2, f3);
}

void ShaderProgram::SetParameter4f(const char* name, float f1, float f2, float f3, float f4)
{
    this->SetParameter4f(this->GetUniformLocation(name), f1, f2, f3, f4);
}

void ShaderProgram::SetParameterMatrix4fv(const char* name, const float* f)
{
    this->SetParameterMatrix4fv(this->GetUniformLocation(name), f);
}

void ShaderProgram::UpdateUniformLocationCache()
{
    _uniformLocationCache[UnderlyingCast(PredefinedUniformLocation::MatrixWVP)] = this->GetUniformLocation("matWVP");
}

void ShaderProgram::ResetUniformCache()
{
    memset(_uniformCache.data(), 0, sizeof(_uniformCache));
}

void ShaderProgram::SetParameterWVPMatrix4fv(const float* f)
{
    const auto location = _uniformLocationCache[UnderlyingCast(PredefinedUniformLocation::MatrixWVP)];
    this->SetParameterMatrix4fv(location, f);
}

}
