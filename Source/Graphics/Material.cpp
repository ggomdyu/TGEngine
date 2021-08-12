#include "PrecompiledHeader.h"

#include "Core/MakeSharedEnabler.h"

#include "Material.h"

namespace tg
{

Material::Material(ShaderProgram&& shaderProgram) noexcept :
    m_shaderProgram(std::move(shaderProgram))
{
}

std::optional<Material> Material::Create(const char* vertexShaderCode, const char* fragmentShaderCode)
{
    auto shaderProgram = ShaderProgram::Create(vertexShaderCode, fragmentShaderCode);
    if (!shaderProgram)
    {
        return {};
    }

    return Material(std::move(*shaderProgram));
}

std::shared_ptr<Material> Material::Create(const char* vertexShaderCode, const char* fragmentShaderCode, ReturnPointerTag)
{
    auto shaderProgram = ShaderProgram::Create(vertexShaderCode, fragmentShaderCode);
    if (!shaderProgram)
    {
        return {};
    }

    return std::make_shared<MakeSharedEnabler<Material>>(std::move(*shaderProgram));
}

void Material::Use()
{
    m_shaderProgram.Use();
}

void Material::Disuse()
{
    m_shaderProgram.Disuse();
}

void Material::SetParameter1f(const char* name, float f)
{
    m_shaderProgram.SetParameter1f(name, f);
}

void Material::SetParameter2f(const char* name, float f1, float f2)
{
    m_shaderProgram.SetParameter2f(name, f1, f2);
}

void Material::SetParameter3f(const char* name, float f1, float f2, float f3)
{
    m_shaderProgram.SetParameter3f(name, f1, f2, f3);
}

void Material::SetParameter4f(const char* name, float f1, float f2, float f3, float f4)
{
    m_shaderProgram.SetParameter4f(name, f1, f2, f3, f4);
}

void Material::SetParameterMatrix4fv(const char* name, const float* f)
{
    m_shaderProgram.SetParameterMatrix4fv(name, f);
}

void Material::SetParameter1f(int32_t location, float f)
{
    m_shaderProgram.SetParameter1f(location, f);
}

void Material::SetParameter2f(int32_t location, float f1, float f2)
{
    m_shaderProgram.SetParameter2f(location, f1, f2);
}

void Material::SetParameter3f(int32_t location, float f1, float f2, float f3)
{
    m_shaderProgram.SetParameter3f(location, f1, f2, f3);
}

void Material::SetParameter4f(int32_t location, float f1, float f2, float f3, float f4)
{
    m_shaderProgram.SetParameter4f(location, f1, f2, f3, f4);
}

void Material::SetParameterMatrix4fv(int32_t location, const float* f)
{
    m_shaderProgram.SetParameterMatrix4fv(location, f);
}

void Material::SetParameterWVPMatrix4fv(const float* f)
{
    m_shaderProgram.SetParameterWVPMatrix4fv(f);
}

void Material::SetParameterSampler(int32_t location, uint32_t textureUnit, uint32_t texture)
{
    m_shaderProgram.SetParameterSampler(location, textureUnit, texture);
}

void Material::BindAttributeLocation(const char* name, uint32_t location)
{
    m_shaderProgram.BindAttributeLocation(name, location);
}

int32_t Material::GetUniformLocation(const char* name) const
{
    return m_shaderProgram.GetUniformLocation(name);
}

}
