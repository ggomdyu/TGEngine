#pragma once

#include "Graphics/Graphics.h"
#include "Graphics/Texture.h"
#include "Math/Rect.h"
#include "Math/Vector2.h"
#include "Math/Matrix4x4.h"

#include "UIElement.h"

namespace tg
{

class UISprite final :
    public UIElement
{
/**@section Constructor */
public:
    UISprite() = default;
    UISprite(const std::shared_ptr<Texture>& texture, BlendMode blendMode = BlendMode::Normal, Color blendColor = Color(1.0f, 1.0f, 1.0f, 1.0f)) noexcept;
    UISprite(UISprite&& rhs) noexcept = default;

/**@section Operator */
public:
    UISprite& operator=(UISprite&& rhs) noexcept = default;
    
/**@section Method */
public:
    void SetTexture(std::shared_ptr<Texture>&& texture) noexcept;
    void SetTexture(const std::shared_ptr<Texture>& texture) noexcept;
    void SetTextureRect(const FRect& textureRect) noexcept;
    void SetTextureSize(const FExtent2D& textureSize) noexcept;
    void SetBlendMode(BlendMode blendMode) noexcept;
    void SetBlendColor(const Color& blendColor) noexcept;
    void SetPivot(const Vector2& pivot) noexcept;
    std::shared_ptr<Texture> GetTexture() noexcept;
    std::shared_ptr<const Texture> GetTexture() const noexcept;
    const FRect& GetTextureRect() const noexcept;
    BlendMode GetBlendMode() const noexcept;
    const Color& GetBlendColor() const noexcept;
    const Vector2& GetPivot() const noexcept;
    void GetBatches(std::vector<UIBatch>* batches, const Matrix4x4& matWorld, std::vector<float>* vertices) const override;
       
/**@section Variable */
private:
    std::shared_ptr<Texture> m_texture;
    FExtent2D m_textureSize;
    FRect m_textureRect;
    BlendMode m_blendMode = BlendMode::Normal;
    Color m_blendColor = Color(1.0f, 1.0f, 1.0f, 1.0f);
    Vector2 m_pivot = Vector2(0.5f, 0.5f);
};

}
