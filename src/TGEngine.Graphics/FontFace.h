#pragma once

#include <span>
#include <unordered_map>
#include <ft2build.h>
#include FT_FREETYPE_H

#include "Core/NonCopyable.h"
#include "Math/Vector2.h"
#include "Math/Extent.h"

TG_NAMESPACE_BEGIN

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

class FontFace
{
public:
    explicit FontFace(std::unique_ptr<std::remove_pointer_t<FT_Face>, void(*)(FT_Face)> fontFace) noexcept;

public:
    static std::shared_ptr<FontFace> Create(const std::shared_ptr<std::remove_pointer_t<FT_Library>>& library, const std::span<const std::byte>& fileData, int32_t fontSize);
    const GlyphData* GetGlyphData(char32_t c) const;
    IntVector2 GetKerning(char32_t lhs, char32_t rhs) const;

private:
    std::unique_ptr<std::remove_pointer_t<FT_Face>, void(*)(FT_Face)> _fontFace;
    mutable std::unordered_map<char32_t, GlyphData> _glyphData;
};

}
