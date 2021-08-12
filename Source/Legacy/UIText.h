#pragma once

#include <string>

#include "Math/Color.h"
#include "Math/Rect.h"
#include "Graphics/FontAtlas.h"

#include "UIElement.h"

namespace tg
{

enum class TextAlignment
{
    UpperLeft,
    UpperCenter,
    UpperRight,
    MiddleLeft,
    MiddleCenter,
    MiddleRight,
    LowerLeft,
    LowerCenter,
    LowerRight
};

enum class LineBreakMode
{
    Clip,
    CharacterWrap,
    TruncateHead,
    TruncateMiddle,
    TruncateTail,
};

struct CharacterInfo
{
    char32_t character;
    I32Rect rect;
    //Color4f color;
};

struct LineInfo
{
    int32_t characterStartIndex;
    int32_t characterEndIndex;
    I32Rect contentRect;
};

class UIText final :
    public UIElement
{
/**@section Constructor */
public:
    UIText();
    UIText(UIText&& rhs) noexcept = default;
    
/**@section Destructor */
public:
    ~UIText() override;

/**@section Operator */
public:
    UIText& operator=(UIText&& rhs) noexcept = default;
    
/**@section Method */
public:
    void SetFontAtlas(const std::shared_ptr<FontAtlas>& fontAtlas);
    void SetFontAtlas(std::shared_ptr<FontAtlas>&& fontAtlas);
    void SetFontSize(int32_t fontSize);
    void SetText(const std::string_view& text);
    void SetTextAlignment(TextAlignment textAlignment);
    void SetLineSpacing(float lineSpacing);
    void SetLineBreakMode(LineBreakMode lineBreakMode);
    void SetBlendColor(const Color& blendColor);
    void SetRect(const I32Rect& rect);
    std::shared_ptr<FontAtlas> GetFontAtlas() noexcept;
    std::shared_ptr<const FontAtlas> GetFontAtlas() const noexcept;
    int32_t GetFontSize() const noexcept;
    const std::string& GetText() const noexcept;
    TextAlignment GetTextAlignment() const noexcept;
    float GetLineSpacing() const noexcept;
    LineBreakMode GetLineBreakMode() const noexcept;
    const Color& GetBlendColor() const noexcept;
    const I32Rect& GetRect() const noexcept;
    const I32Rect& GetContentRect() const noexcept;
    const std::vector<CharacterInfo>& GetCharacterInfos() const;
    void GetBatches(std::vector<UIBatch>* batches, const Matrix4x4& matWorld, std::vector<float>* vertices) const override;
    
/**@section Variable */
public:
    static constexpr int32_t DefaultFontSize = 12;
    std::string m_text;
    std::shared_ptr<FontAtlas> m_fontAtlas;
    int32_t m_fontSize = DefaultFontSize;
    TextAlignment m_textAlignment = TextAlignment::UpperLeft;
    float m_lineSpacing = 0.0f;
    LineBreakMode m_lineBreakMode = LineBreakMode::CharacterWrap;
    Color m_blendColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
    I32Rect m_rect;
    mutable bool m_isDirty = true;
    std::unique_ptr<class TextBlock> m_textBlock;
};

}
