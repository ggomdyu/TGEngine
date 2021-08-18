#include "FontAtlas.h"

TG_NAMESPACE_BEGIN

FontAtlas::FontAtlas(const std::shared_ptr<Font>& font) :
    _font(font)
{
}

FontAtlas::FontAtlas(std::shared_ptr<Font>&& font) :
    _font(std::move(font))
{
}

FontAtlas::FontAtlas(FontAtlas&& rhs) noexcept :
    _font(std::move(rhs._font)),
    _textureAtlas(std::move(rhs._textureAtlas))
{
}

FontAtlas& FontAtlas::operator=(FontAtlas&& rhs) noexcept
{
    _font = std::move(rhs._font);
    _textureAtlas = std::move(rhs._textureAtlas);
    
    return *this;
}

void FontAtlas::Initialize(const std::shared_ptr<Font>& font)
{
    _textureAtlas.Initialize(FontAtlas::DefaultAtlasSize, FontAtlas::DefaultPixelFormat, FontAtlas::DefaultPaddingOffset);
    _font = font;
}

void FontAtlas::Initialize(std::shared_ptr<Font>&& font)
{
    _textureAtlas.Initialize(FontAtlas::DefaultAtlasSize, FontAtlas::DefaultPixelFormat, FontAtlas::DefaultPaddingOffset);
    _font = std::move(font);
}

std::optional<Rect> FontAtlas::GetTextureRect(char32_t c, int32_t fontSize) const
{
    const auto textureAtlasKey = CreateTextureAtlasKey(c, fontSize);
    return _textureAtlas.GetTextureRect(textureAtlasKey);
}

std::shared_ptr<const Texture> FontAtlas::GetAtlasTexture() const noexcept
{
    return _textureAtlas.GetAtlasTexture();
}

std::shared_ptr<Texture> FontAtlas::GetAtlasTexture() noexcept
{
    return _textureAtlas.GetAtlasTexture();
}

const GlyphData* FontAtlas::GetGlyphData(char32_t c, int32_t fontSize) const
{
    const auto glyphData = _font->GetFontFace(fontSize)->GetGlyphData(c);
    if (glyphData != nullptr)
    {
        const auto textureAtlasKey = CreateTextureAtlasKey(c, fontSize);
        if (_textureAtlas.Contains(textureAtlasKey) == false)
        {
            _textureAtlas.Insert(textureAtlasKey, &glyphData->bitmap[0], glyphData->metrics.size);
        }
    }
    
    return glyphData;
}

IntVector2 FontAtlas::GetKerning(char32_t lhs, char32_t rhs, int32_t fontSize) const
{
    return _font->GetFontFace(fontSize)->GetKerning(lhs, rhs);
}

FontAtlas::TextureAtlasKey FontAtlas::CreateTextureAtlasKey(char32_t c, int32_t fontSize) noexcept
{
    return static_cast<TextureAtlasKey>(c) << 32 | fontSize;
}

}
