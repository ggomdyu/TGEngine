#pragma once

#include "Math/Vector2.h"
#include "Drawing/Image.h"

#if TG_GRAPHICS_OPENGL
#include "OpenGL/OpenGLTexture.h"
#endif

TG_NAMESPACE_BEGIN

enum class FilterMode
{
    Point,
    Linear,
    Trilinear
};

enum class WrapMode
{
    Repeat,
    Clamp,
    Mirror,
};

class Texture final :
    private PlatformTexture
{
public:
    Texture(const Image& image, FilterMode filterMode, WrapMode wrapMode, bool isUseMipmap, bool isDynamicUsage);
    Texture(const std::byte* imageData, const I32Extent2D& size, PixelFormat pixelFormat, FilterMode filterMode, WrapMode wrapMode, bool isUseMipmap, bool isDynamicUsage);
    Texture(Texture&& rhs) noexcept = default;

public:
    Texture& operator=(Texture&& rhs) noexcept = default;
  
public:
    PlatformTexture& GetPlatformDependency() noexcept;
    const PlatformTexture& GetPlatformDependency() const noexcept;
    void Use();
    void Unuse();
    void SetData(const std::byte* imageData, const I32Extent2D& size, PixelFormat pixelFormat);
    void SetData(const std::byte* imageData, const Vector2& pos, const I32Extent2D& size, PixelFormat pixelFormat);
    void SetFilterMode(FilterMode filterMode);
    void SetWrapMode(WrapMode wrapMode);
    const I32Extent2D& GetSize() const noexcept;
    PixelFormat GetPixelFormat() const noexcept;
    FilterMode GetFilterMode() const noexcept;
    WrapMode GetWrapMode() const noexcept;
    bool IsValid() const;
    bool IsUseMipmap() const noexcept;
    
protected:
    bool _isUseMipmap = false;
    bool _isDynamicUsage = false;
    PixelFormat _pixelFormat = PixelFormat::RGBA8888;
    FilterMode _filterMode = FilterMode::Linear;
    WrapMode _wrapMode = WrapMode::Clamp;
    I32Extent2D _size;
    inline static Texture* _latelyUsedTexture;
};
    
}
