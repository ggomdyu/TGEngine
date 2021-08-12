#pragma once

#include "Drawing/Font.h"
#include "Graphics/TextureAtlas.h"

namespace tg
{

class FontAtlas final :
    private NonCopyable
{
/**@section Type */
public:
    using TextureAtlasKey = uint64_t;

/**@section Constructor */
public:
    FontAtlas() = default;
    explicit FontAtlas(const std::shared_ptr<Font>& font);
    explicit FontAtlas(std::shared_ptr<Font>&& font);
    FontAtlas(FontAtlas&& rhs) noexcept;
        
/**@section Operator */
public:
    FontAtlas& operator=(FontAtlas&& rhs) noexcept;

/**@section Method */
public:
    void Initialize(const std::shared_ptr<Font>& font);
    void Initialize(std::shared_ptr<Font>&& font);
    std::optional<Rect> GetTextureRect(char32_t c, int32_t fontSize) const;
    std::shared_ptr<Texture> GetAtlasTexture() noexcept;
    std::shared_ptr<const Texture> GetAtlasTexture() const noexcept;
    const GlyphData* GetGlyphData(char32_t c, int32_t fontSize) const;
    IntVector2 GetKerning(char32_t lhs, char32_t rhs, int32_t fontSize) const;

private:
    static TextureAtlasKey CreateTextureAtlasKey(char32_t c, int32_t fontSize) noexcept;
    
/**@section Variable */
private:
    static constexpr auto DefaultAtlasSize = I32Extent2D(1024, 1024);
    static constexpr auto DefaultPixelFormat = PixelFormat::RGBA8888;
    static constexpr auto DefaultPaddingOffset = 2;

    std::shared_ptr<Font> m_font;
    mutable BasicTextureAtlas<TextureAtlasKey> m_textureAtlas = {FontAtlas::DefaultAtlasSize, FontAtlas::DefaultPixelFormat, FontAtlas::DefaultPaddingOffset};
};

}
