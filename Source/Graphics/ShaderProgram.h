#pragma once

#include <array>

#include "Math/Vector4.h"

#if TGON_GRAPHICS_OPENGL
#include "OpenGL/OpenGLShaderProgram.h"
#endif

namespace tg
{

class ShaderProgram final :
    private PlatformShaderProgram
{
/**@section Enum */
private:
    enum class PredefinedUniformLocation
    {
        MatrixWVP,
        Sampler,
        BlendColor,
        Ambient,
        Specular,
        Diffuse,
        Emission,
    };

/**@section Constructor */
public:
    using PlatformShaderProgram::PlatformShaderProgram;
    ShaderProgram(const ShaderProgram& rhs) = delete;
    ShaderProgram(ShaderProgram&& rhs) noexcept;
    
/**@section Destructor */
public:
    ~ShaderProgram() = default;

/**@section Operator */
public:
    ShaderProgram& operator=(const ShaderProgram& rhs) = delete;
    ShaderProgram& operator=(ShaderProgram&& rhs) noexcept;
   
/**@section Method */
public:
    [[nodiscard]] PlatformShaderProgram& GetPlatformDependency() noexcept;
    [[nodiscard]] const PlatformShaderProgram& GetPlatformDependency() const noexcept;
    [[nodiscard]] static std::optional<ShaderProgram> Create(const char* vertexShaderCode, const char* fragmentShaderCode);
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

private:
    void UpdateUniformLocationCache();
    void ResetUniformCache();

/**@section Variable */
private:
    std::array<Vector4, 64> m_uniformCache{};
    std::array<int32_t, 8> m_uniformLocationCache{};
};

}
