#if TG_GRAPHICS_OPENGL
#include "Core/Algorithm.h"

#include "OpenGLDebug.h"

#include "../Texture.h"

TG_NAMESPACE_BEGIN
namespace
{

constexpr GLint ConvertTextureFilterModeToNative(FilterMode textureFilterMode, bool isUseMipmap) noexcept
{
    auto nativeTextureFilterModes = (isUseMipmap) ?
        MakeArray(GL_NEAREST_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_NEAREST, GL_LINEAR_MIPMAP_LINEAR) :
        MakeArray(GL_NEAREST, GL_LINEAR, GL_LINEAR);

    return nativeTextureFilterModes[UnderlyingCast(textureFilterMode)];
}
    
constexpr GLint ConvertTextureWrapModeToNative(WrapMode textureWrapMode) noexcept
{
    constexpr GLint nativeTextureWrapModes[] = {
        GL_REPEAT,
        GL_CLAMP_TO_EDGE,
        GL_MIRRORED_REPEAT,
    };
    return nativeTextureWrapModes[UnderlyingCast(textureWrapMode)];
}

constexpr GLenum ConvertPixelFormatToNative(PixelFormat pixelFormat) noexcept
{
    constexpr GLenum nativePixelFormats[] = {
        0,
        GL_RGBA,
        GL_RGB,
        GL_RGB4,
        GL_RED,
    };              
    return nativePixelFormats[UnderlyingCast(pixelFormat)];
}

GLuint CreateTextureHandle()
{
    GLuint textureHandle;
    TG_GL_ERROR_CHECK(glGenTextures(1, &textureHandle));

    return textureHandle;
}

GLuint CreatePixelBufferHandle(GLsizeiptr bufferBytes)
{
    GLuint pixelBufferHandle = 0;

    TG_GL_ERROR_CHECK(glGenBuffers(1, &pixelBufferHandle));
    TG_GL_ERROR_CHECK(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, pixelBufferHandle));
    TG_GL_ERROR_CHECK(glBufferData(GL_PIXEL_UNPACK_BUFFER, bufferBytes, nullptr, GL_STATIC_DRAW));
    TG_GL_ERROR_CHECK(glBindBuffer(GL_PIXEL_UNPACK_BUFFER, 0));

    return pixelBufferHandle;
}

}

OpenGLTexture::OpenGLTexture() :
    _textureHandle(CreateTextureHandle())
{
}

OpenGLTexture::OpenGLTexture(OpenGLTexture&& rhs) noexcept :
    _textureHandle(rhs._textureHandle)
{
    rhs._textureHandle = 0;
}

OpenGLTexture::~OpenGLTexture()
{
    this->Destroy();
}

OpenGLTexture& OpenGLTexture::operator=(OpenGLTexture&& rhs) noexcept
{
    std::swap(_textureHandle, rhs._textureHandle);
    return *this;
}

void OpenGLTexture::Use()
{
    TG_GL_ERROR_CHECK(glBindTexture(GL_TEXTURE_2D, _textureHandle));
    this->UpdateTexParameters();
}

void OpenGLTexture::Unuse()
{
    TG_GL_ERROR_CHECK(glBindTexture(GL_TEXTURE_2D, 0));
}

void OpenGLTexture::CreateMipmap() const
{
    TG_GL_ERROR_CHECK(glGenerateMipmap(GL_TEXTURE_2D));
}

void OpenGLTexture::Destroy()
{
    if (_textureHandle != 0)
    {
        TG_GL_ERROR_CHECK(glDeleteTextures(1, &_textureHandle));
        _textureHandle = 0;
    }
}

void OpenGLTexture::UpdateTexParameters()
{
    // Update the texture filter
    auto downcastedMe = reinterpret_cast<Texture*>(this);
    TG_GL_ERROR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, ConvertTextureFilterModeToNative(downcastedMe->GetFilterMode(), downcastedMe->IsUseMipmap())));
    TG_GL_ERROR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR));

    // Update the texture wrap mode
    auto wrapMode = ConvertTextureWrapModeToNative(reinterpret_cast<Texture*>(this)->GetWrapMode());
    TG_GL_ERROR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapMode));
    TG_GL_ERROR_CHECK(glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapMode));
}

void Texture::SetData(const std::byte* imageData, const I32Extent2D& size, PixelFormat pixelFormat)
{
    auto nativePixelFormat = ConvertPixelFormatToNative(pixelFormat);
    
    TG_GL_ERROR_CHECK(glBindTexture(GL_TEXTURE_2D, _textureHandle));
    TG_GL_ERROR_CHECK(glTexImage2D(GL_TEXTURE_2D, 0, nativePixelFormat, size.width, size.height, 0, nativePixelFormat, GL_UNSIGNED_BYTE, imageData));

    if (_isUseMipmap)
    {
        this->CreateMipmap();
    }
}

void Texture::SetData(const std::byte* imageData, const Vector2& pos, const I32Extent2D& size, PixelFormat pixelFormat)
{
    TG_GL_ERROR_CHECK(glBindTexture(GL_TEXTURE_2D, _textureHandle));
    TG_GL_ERROR_CHECK(glTexSubImage2D(GL_TEXTURE_2D, 0, static_cast<GLint>(pos.x), static_cast<GLint>(pos.y), static_cast<GLsizei>(size.width), static_cast<GLsizei>(size.height), ConvertPixelFormatToNative(pixelFormat), GL_UNSIGNED_BYTE, &imageData[0]));

    if (_isUseMipmap)
    {
        this->CreateMipmap();
    }
}

bool Texture::IsValid() const
{
    return glIsTexture(_textureHandle);
}

bool Texture::IsUseMipmap() const noexcept
{
    return _isUseMipmap;
}

}

#endif
