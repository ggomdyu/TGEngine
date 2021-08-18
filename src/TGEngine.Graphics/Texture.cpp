#include "Texture.h"

TG_NAMESPACE_BEGIN

Texture::Texture(const Image& image, FilterMode filterMode, WrapMode wrapMode, bool isUseMipmap, bool isDynamicUsage) :
    Texture(image.GetImageData(), image.GetSize(), image.GetPixelFormat(), filterMode, wrapMode, isUseMipmap, isDynamicUsage)
{
}

Texture::Texture(const std::byte* imageData, const I32Extent2D& size, PixelFormat pixelFormat, FilterMode filterMode, WrapMode wrapMode, bool isUseMipmap, bool isDynamicUsage) :
    PlatformTexture(),
    _isUseMipmap(isUseMipmap),
    _isDynamicUsage(isDynamicUsage),
    _pixelFormat(pixelFormat),
    _filterMode(filterMode),
    _wrapMode(wrapMode),
    _size(size)
{
    this->SetData(imageData, size, pixelFormat);
}

PlatformTexture& Texture::GetPlatformDependency() noexcept
{
    return *this;
}

const PlatformTexture& Texture::GetPlatformDependency() const noexcept
{
    return *this;
}

void Texture::Use()
{
    if (_latelyUsedTexture == this)
    {
        return;
    }

    PlatformTexture::Use();
    _latelyUsedTexture = this;
}

void Texture::Unuse()
{
    PlatformTexture::Unuse();
    _latelyUsedTexture = nullptr;
}

void Texture::SetFilterMode(FilterMode filterMode)
{
    _filterMode = filterMode;
}

void Texture::SetWrapMode(WrapMode wrapMode)
{
    _wrapMode = wrapMode;
}

FilterMode Texture::GetFilterMode() const noexcept
{
    return _filterMode;
}

WrapMode Texture::GetWrapMode() const noexcept
{
    return _wrapMode;
}

const I32Extent2D& Texture::GetSize() const noexcept
{
    return _size;
}
    
PixelFormat Texture::GetPixelFormat() const noexcept
{
    return _pixelFormat;
}

}
