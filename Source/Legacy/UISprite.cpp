#include "PrecompiledHeader.h"

#include "Graphics/Texture.h"

#include "UISprite.h"

namespace tg
{

UISprite::UISprite(const std::shared_ptr<Texture>& texture, BlendMode blendMode, Color blendColor) noexcept :
    m_texture(texture),
    m_textureSize(texture ? FExtent2D(texture->GetSize()) : FExtent2D(0.0f, 0.0f)),
    m_textureRect(0, 0, m_textureSize.width, m_textureSize.height),
    m_blendMode(blendMode),
    m_blendColor(blendColor)
{
}

void UISprite::SetTexture(std::shared_ptr<Texture>&& texture) noexcept
{
    m_textureSize = texture ? FExtent2D(texture->GetSize()) : FExtent2D(0.0f, 0.0f);
    m_textureRect = FRect(0, 0, m_textureSize.width, m_textureSize.height);
    m_texture = std::move(texture);
}

void UISprite::SetTexture(const std::shared_ptr<Texture>& texture) noexcept
{
    m_textureSize = texture ? FExtent2D(texture->GetSize()) : FExtent2D(0.0f, 0.0f);
    m_textureRect = FRect(0, 0, m_textureSize.width, m_textureSize.height);
    m_texture = texture;
}

void UISprite::SetTextureSize(const FExtent2D& textureSize) noexcept
{
    m_textureSize = textureSize;
}

void UISprite::SetTextureRect(const FRect& textureRect) noexcept
{
    m_textureRect = textureRect;
}

void UISprite::SetBlendMode(BlendMode blendMode) noexcept
{
    m_blendMode = blendMode;
}

void UISprite::SetBlendColor(const Color& blendColor) noexcept
{
    m_blendColor = blendColor;
}

void UISprite::SetPivot(const Vector2& pivot) noexcept
{
    m_pivot = pivot;
}

std::shared_ptr<Texture> UISprite::GetTexture() noexcept
{
    return m_texture;
}

std::shared_ptr<const Texture> UISprite::GetTexture() const noexcept
{
    return m_texture;
}

const FRect& UISprite::GetTextureRect() const noexcept
{
    return m_textureRect;
}

BlendMode UISprite::GetBlendMode() const noexcept
{
    return m_blendMode;
}

const Color& UISprite::GetBlendColor() const noexcept
{
    return m_blendColor;
}

const Vector2& UISprite::GetPivot() const noexcept
{
    return m_pivot;
}

void UISprite::GetBatches(std::vector<UIBatch>* batches, const Matrix4x4& matWorld, std::vector<float>* vertices) const
{
    UIBatch batch(m_material ? m_material : m_defaultMaterial, m_texture, m_blendMode, static_cast<int32_t>(vertices->size()));
    if (batches->empty() || batches->back().CanBatch(batch) == false)
    {
        batches->push_back(batch);
    }

    batches->back().Merge(m_textureRect, m_textureSize, m_pivot, m_blendColor, matWorld, vertices);
}

}
