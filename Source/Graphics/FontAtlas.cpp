#include "PrecompiledHeader.h"

#include "FontAtlas.h"

namespace tg
{

FontAtlas::FontAtlas(const std::shared_ptr<Font>& font) :
    m_font(font)
{
}

FontAtlas::FontAtlas(std::shared_ptr<Font>&& font) :
    m_font(std::move(font))
{
}

FontAtlas::FontAtlas(FontAtlas&& rhs) noexcept :
    m_font(std::move(rhs.m_font)),
    m_textureAtlas(std::move(rhs.m_textureAtlas))
{
}

FontAtlas& FontAtlas::operator=(FontAtlas&& rhs) noexcept
{
    m_font = std::move(rhs.m_font);
    m_textureAtlas = std::move(rhs.m_textureAtlas);
    
    return *this;
}

void FontAtlas::Initialize(const std::shared_ptr<Font>& font)
{
    m_textureAtlas.Initialize(FontAtlas::DefaultAtlasSize, FontAtlas::DefaultPixelFormat, FontAtlas::DefaultPaddingOffset);
    m_font = font;
}

void FontAtlas::Initialize(std::shared_ptr<Font>&& font)
{
    m_textureAtlas.Initialize(FontAtlas::DefaultAtlasSize, FontAtlas::DefaultPixelFormat, FontAtlas::DefaultPaddingOffset);
    m_font = std::move(font);
}

std::optional<Rect> FontAtlas::GetTextureRect(char32_t c, int32_t fontSize) const
{
    const auto textureAtlasKey = CreateTextureAtlasKey(c, fontSize);
    return m_textureAtlas.GetTextureRect(textureAtlasKey);
}

std::shared_ptr<const Texture> FontAtlas::GetAtlasTexture() const noexcept
{
    return m_textureAtlas.GetAtlasTexture();
}

std::shared_ptr<Texture> FontAtlas::GetAtlasTexture() noexcept
{
    return m_textureAtlas.GetAtlasTexture();
}

const GlyphData* FontAtlas::GetGlyphData(char32_t c, int32_t fontSize) const
{
    const auto glyphData = m_font->GetFontFace(fontSize)->GetGlyphData(c);
    if (glyphData != nullptr)
    {
        const auto textureAtlasKey = CreateTextureAtlasKey(c, fontSize);
        if (m_textureAtlas.Contains(textureAtlasKey) == false)
        {
            m_textureAtlas.Insert(textureAtlasKey, &glyphData->bitmap[0], glyphData->metrics.size);
        }
    }
    
    return glyphData;
}

IntVector2 FontAtlas::GetKerning(char32_t lhs, char32_t rhs, int32_t fontSize) const
{
    return m_font->GetFontFace(fontSize)->GetKerning(lhs, rhs);
}

FontAtlas::TextureAtlasKey FontAtlas::CreateTextureAtlasKey(char32_t c, int32_t fontSize) noexcept
{
    return static_cast<TextureAtlasKey>(c) << 32 | fontSize;
}

}
