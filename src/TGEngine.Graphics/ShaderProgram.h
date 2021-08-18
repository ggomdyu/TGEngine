#pragma once

#include <array>

#include "Math/Vector4.h"

#if TG_GRAPHICS_OPENGL
#include "OpenGL/OpenGLShaderProgram.h"
#endif

TG_NAMESPACE_BEGIN

class ShaderProgram final :
    private PlatformShaderProgram
{
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

public:
    using PlatformShaderProgram::PlatformShaderProgram;
    ShaderProgram(const ShaderProgram& rhs) = delete;
    ShaderProgram(ShaderProgram&& rhs) noexcept;
    ~ShaderProgram() = default;

public:
    ShaderProgram& operator=(const ShaderProgram& rhs) = delete;
    ShaderProgram& operator=(ShaderProgram&& rhs) noexcept;
   
public:
    PlatformShaderProgram& GetPlatformDependency() noexcept;
    const PlatformShaderProgram& GetPlatformDependency() const noexcept;
    static std::optional<ShaderProgram> Create(const char* vertexShaderCode, const char* fragmentShaderCode);
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
    int32_t GetUniformLocation(const char* name) const;

private:
    void UpdateUniformLocationCache();
    void ResetUniformCache();

private:
    std::array<Vector4, 64> _uniformCache{};
    std::array<int32_t, 8> _uniformLocationCache{};
};

TG_NAMESPACE_END