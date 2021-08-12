#include "PrecompiledHeader.h"

#include "Core/MakeSharedEnabler.h"

#include "FontFace.h"

namespace tg
{

FontFace::FontFace(std::unique_ptr<std::remove_pointer_t<FT_Face>, void(*)(FT_Face)> fontFace) noexcept :
    m_fontFace(std::move(fontFace))
{
}

std::shared_ptr<FontFace> FontFace::Create(const std::shared_ptr<std::remove_pointer_t<FT_Library>>& library, const std::span<const std::byte>& fileData, int32_t fontSize)
{
    FT_Face face = nullptr;
    if (FT_New_Memory_Face(library.get(), reinterpret_cast<const FT_Byte*>(fileData.data()), static_cast<FT_Long>(fileData.size()), 0, &face))
    {
        return {};
    }

    if (FT_Select_Charmap(face, FT_ENCODING_UNICODE))
    {
        return {};
    }

    if (FT_Set_Pixel_Sizes(face, 0, fontSize))
    {
        return {};
    }

    return std::make_shared<MakeSharedEnabler<FontFace>>(decltype(m_fontFace)(face, [](FT_Face face)
    {
        FT_Done_Face(face);
    }));
}

const GlyphData* FontFace::GetGlyphData(char32_t c) const
{
    const auto it = m_glyphData.find(c);
    if (it != m_glyphData.cend())
    {
        return &it->second;
    }

    if (FT_Load_Char(m_fontFace.get(), c, FT_LOAD_RENDER))
    {
        return nullptr;
    }

    constexpr std::byte whiteColorPixel[4] {std::byte(255), std::byte(255) ,std::byte(255), std::byte(255)};

    const I32Extent2D bitmapExtent(static_cast<int32_t>(m_fontFace->glyph->bitmap.width), static_cast<int32_t>(m_fontFace->glyph->bitmap.rows));
    auto bitmap = std::make_unique<std::byte[]>(static_cast<size_t>(bitmapExtent.width) * bitmapExtent.height * 4);
    for (int64_t i = 0, j = 0; i < static_cast<int64_t>(bitmapExtent.width) * bitmapExtent.height * 4; i += 4, ++j)
    {
        memcpy(&bitmap[i], whiteColorPixel, sizeof(whiteColorPixel));
        bitmap[i + 3] = std::byte(m_fontFace->glyph->bitmap.buffer[j]);
    }

    const auto bearing = IntVector2(static_cast<int32_t>(m_fontFace->glyph->bitmap_left), static_cast<int32_t>(m_fontFace->glyph->bitmap_top));
    const auto advance = IntVector2(static_cast<int32_t>(m_fontFace->glyph->advance.x >> 6), static_cast<int32_t>(m_fontFace->glyph->advance.y >> 6));
    return &m_glyphData.insert(it, {c, GlyphData{c, GlyphMetrics{bitmapExtent, bearing, advance}, std::move(bitmap)}})->second;
}

IntVector2 FontFace::GetKerning(char32_t lhs, char32_t rhs) const
{
    const auto lhsIndex = FT_Get_Char_Index(m_fontFace.get(), lhs);
    const auto rhsIndex = FT_Get_Char_Index(m_fontFace.get(), rhs);

    FT_Vector kerning;
    if (FT_Get_Kerning(m_fontFace.get(), lhsIndex, rhsIndex, FT_KERNING_DEFAULT, &kerning))
    {
        return {};
    }

    return IntVector2(static_cast<int32_t>(kerning.x >> 6), static_cast<int32_t>(kerning.y >> 6));
}

}
