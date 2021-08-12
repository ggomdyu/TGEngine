#pragma once

#include <span>
#include <unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Core/NonCopyable.h"
#include "Math/Vector2.h"
#include "Math/Extent.h"

namespace tg
{

struct GlyphMetrics final
{
    I32Extent2D size;
    IntVector2 bearing;
    IntVector2 advance;
};

struct GlyphData final
{
    char32_t character;
    GlyphMetrics metrics;
    std::unique_ptr<std::byte[]> bitmap;
};

class FontFace :
    private NonCopyable
{
/**@section Constructor */
protected:
    explicit FontFace(std::unique_ptr<std::remove_pointer_t<FT_Face>, void(*)(FT_Face)> fontFace) noexcept;

/**@section Method */
public:
    /**
     * @brief   Creates a instance of object.
     * @return  The instantiated object.
     */
    static std::shared_ptr<FontFace> Create(const std::shared_ptr<std::remove_pointer_t<FT_Library>>& library, const std::span<const std::byte>& fileData, int32_t fontSize);

    /**
     * @brief   Gets the glyph data of the specified character.
     * @param c    Glyph identifier character.
     * @return  The character glyph data or nullptr.
     */
    const GlyphData* GetGlyphData(char32_t c) const;

    /**
     * @brief   Gets the size of kerning between two characters.
     * @param lhs   Left side character.
     * @param rhs   Right side character.
     * @return  Size of kerning.
     */
    IntVector2 GetKerning(char32_t lhs, char32_t rhs) const;

/**@section Variable */
private:
    std::unique_ptr<std::remove_pointer_t<FT_Face>, void(*)(FT_Face)> m_fontFace;
    mutable std::unordered_map<char32_t, GlyphData> m_glyphData;
};

}
