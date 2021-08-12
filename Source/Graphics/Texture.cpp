#include "PrecompiledHeader.h"

#include "Texture.h"

namespace tg
{

Texture::Texture(const Image& image, FilterMode filterMode, WrapMode wrapMode, bool isUseMipmap, bool isDynamicUsage) :
    Texture(image.GetImageData(), image.GetSize(), image.GetPixelFormat(), filterMode, wrapMode, isUseMipmap, isDynamicUsage)
{
}

Texture::Texture(const std::byte* imageData, const I32Extent2D& size, PixelFormat pixelFormat, FilterMode filterMode, WrapMode wrapMode, bool isUseMipmap, bool isDynamicUsage) :
    PlatformTexture(),
    m_isUseMipmap(isUseMipmap),
    m_isDynamicUsage(isDynamicUsage),
    m_pixelFormat(pixelFormat),
    m_filterMode(filterMode),
    m_wrapMode(wrapMode),
    m_size(size)
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
    if (m_latelyUsedTexture == this)
    {
        return;
    }

    PlatformTexture::Use();
    m_latelyUsedTexture = this;
}

void Texture::Unuse()
{
    PlatformTexture::Unuse();
    m_latelyUsedTexture = nullptr;
}

void Texture::SetFilterMode(FilterMode filterMode)
{
    m_filterMode = filterMode;
}

void Texture::SetWrapMode(WrapMode wrapMode)
{
    m_wrapMode = wrapMode;
}

FilterMode Texture::GetFilterMode() const noexcept
{
    return m_filterMode;
}

WrapMode Texture::GetWrapMode() const noexcept
{
    return m_wrapMode;
}

const I32Extent2D& Texture::GetSize() const noexcept
{
    return m_size;
}
    
PixelFormat Texture::GetPixelFormat() const noexcept
{
    return m_pixelFormat;
}

}
