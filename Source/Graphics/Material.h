#pragma once

#include <memory>

#include "Core/TagDispatch.h"

#include "ShaderProgram.h"

namespace tg
{

class Material
{
/**@section Constructor */
protected:
    explicit Material(ShaderProgram&& shaderProgram) noexcept;

/**@section Method */
public:
    [[nodiscard]] static std::optional<Material> Create(const char* vertexShaderCode, const char* fragmentShaderCode);
    [[nodiscard]] static std::shared_ptr<Material> Create(const char* vertexShaderCode, const char* fragmentShaderCode, ReturnPointerTag);
    void Use();
    void Disuse();
    void SetParameter1f(const char* name, float f);
    void SetParameter2f(const char* name, float f1, float f2);
    void SetParameter3f(const char* name, float f1, float f2, float f3);
    void SetParameter4f(const char* name, float f1, float f2, float f3, float f4);
    void SetParameterMatrix4fv(const char* name, const float* f);
    void SetParameter1f(int32_t location, float f);
    void SetParameter2f(int32_t location, float f1, float f2);
    void SetParameter3f(int32_t location, float f1, float f2, float f3);
    void SetParameter4f(int32_t location, float f1, float f2, float f3, float f4);
    void SetParameterMatrix4fv(int32_t location, const float* f);
    void SetParameterWVPMatrix4fv(const float* f);
    void SetParameterSampler(int32_t location, uint32_t textureUnit, uint32_t texture);
    void BindAttributeLocation(const char* name, uint32_t location);
    [[nodiscard]] int32_t GetUniformLocation(const char* name) const;
    
/**@section Variable */
protected:
    ShaderProgram m_shaderProgram;
};

}
