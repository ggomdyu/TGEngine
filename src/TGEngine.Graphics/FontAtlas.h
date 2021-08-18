#pragma once

#include "Drawing/Font.h"
#include "Graphics/TextureAtlas.h"

TG_NAMESPACE_BEGIN

class FontAtlas final :
    private NonCopyable
{
public:
    using TextureAtlasKey = uint64_t;

public:
    FontAtlas() = default;
    explicit FontAtlas(const std::shared_ptr<Font>& font);
    explicit FontAtlas(std::shared_ptr<Font>&& font);
    FontAtlas(FontAtlas&& rhs) noexcept;
        
public:
    FontAtlas& operator=(FontAtlas&& rhs) noexcept;

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
    
private:
    static constexpr auto DefaultAtlasSize = I32Extent2D(1024, 1024);
    static constexpr auto DefaultPixelFormat = PixelFormat::RGBA8888;
    static constexpr auto DefaultPaddingOffset = 2;

    std::shared_ptr<Font> _font;
    mutable BasicTextureAtlas<TextureAtlasKey> _textureAtlas = {FontAtlas::DefaultAtlasSize, FontAtlas::DefaultPixelFormat, FontAtlas::DefaultPaddingOffset};
};

}
