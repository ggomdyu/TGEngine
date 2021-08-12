#include "PrecompiledHeader.h"

#include "Math/Vector4.h"
#include "Graphics/Texture.h"
#include "Graphics/Graphics.h"
#include "Graphics/Material.h"
#include "Graphics/VertexFormat.h"

#include "UIBatch.h"

namespace tg
{
    
UIBatch::UIBatch(const std::shared_ptr<Material>& material, const std::shared_ptr<Texture>& texture, BlendMode blendMode, int32_t vertexStartOffset) noexcept :
    m_material(material),
    m_texture(texture),
    m_blendMode(blendMode),
    m_vertexStartOffset(vertexStartOffset),
    m_vertexEndOffset(vertexStartOffset)
{
}

bool UIBatch::CanBatch(const UIBatch& rhs) const noexcept
{
    if (m_texture == rhs.m_texture && m_material == rhs.m_material && m_blendMode == rhs.m_blendMode)
    {
        return true;
    }
    
    return false;
}
    
void UIBatch::FlushBatch(Graphics& graphics)
{
    m_material->Use();
    m_texture->Use();
    
    graphics.DrawPrimitives(PrimitiveType::Triangles, m_vertexStartOffset / (sizeof(V3F_C4F_T2F) / 4), (m_vertexEndOffset - m_vertexStartOffset) / static_cast<int32_t>(sizeof(V3F_C4F_T2F) / 4));
}

std::shared_ptr<Material> UIBatch::GetMaterial() noexcept
{
    return m_material;
}

std::shared_ptr<Texture> UIBatch::GetTexture() noexcept
{
    return m_texture;
}

std::shared_ptr<const Material> UIBatch::GetMaterial() const noexcept
{
    return m_material;
}

std::shared_ptr<const Texture> UIBatch::GetTexture() const noexcept
{
    return m_texture;
}

BlendMode UIBatch::GetBlendMode() const noexcept
{
    return m_blendMode;
}

void UIBatch::Merge(float x, float y, const FRect& textureRect, const Vector2& pivot, const Color& blendColor, const Matrix4x4& matWorld, std::vector<float>* vertices)
{
    decltype(auto) textureSize = m_texture->GetSize();
        
    const float leftUV = textureRect.x / textureSize.width;
    const float rightUV = (textureRect.x + textureRect.width) / textureSize.width;
#if TGON_GRAPHICS_OPENGL
    const float topUV = textureRect.y / textureSize.height;
    const float bottomUV = (textureRect.y + textureRect.height) / textureSize.height;
#else
    const float topUV = (textureRect.y + textureRect.height) / textureSize.height;
    const float bottomUV = textureRect.y / textureSize.height;
#endif
    const float halfWidth = 0.5f * textureRect.width;
    const float halfHeight = 0.5f * textureRect.height;
    const float xOffset = x + (pivot.x - 0.5f) * -textureRect.width;
    const float yOffset = y + (pivot.y - 0.5f) * textureRect.height;

    const int32_t oldVertexEndOffset = m_vertexEndOffset;
    const int32_t expandSize = sizeof(V3F_C4F_T2F) / 4 * 6;
    vertices->resize(static_cast<size_t>(m_vertexEndOffset) + expandSize);
    m_vertexEndOffset += expandSize;

    // Left top
    V3F_C4F_T2F* newVertices = reinterpret_cast<V3F_C4F_T2F*>(&(*vertices)[oldVertexEndOffset]);
    newVertices[0].position = Vector4(-halfWidth + xOffset, halfHeight + yOffset, 0.0f, 1.0f) * matWorld;
    newVertices[0].color = blendColor;
    newVertices[0].uv = {leftUV, topUV};

    // Right top
    newVertices[1].position = Vector4(halfWidth + xOffset, halfHeight + yOffset, 0.0f, 1.0f) * matWorld;
    newVertices[1].color = blendColor;
    newVertices[1].uv = {rightUV, topUV};

    // Right bottom
    newVertices[2].position = Vector4(halfWidth + xOffset, -halfHeight + yOffset, 0.0f, 1.0f) * matWorld;
    newVertices[2].color = blendColor;
    newVertices[2].uv = {rightUV, bottomUV};

    // Right bottom
    newVertices[3] = newVertices[2];

    // Left bottom
    newVertices[4].position = Vector4(-halfWidth + xOffset, -halfHeight + yOffset, 0.0f, 1.0f) * matWorld;
    newVertices[4].color = blendColor;
    newVertices[4].uv = {leftUV, bottomUV};

    // Left top
    newVertices[5] = newVertices[0];
}
    
void UIBatch::Merge(const FRect& textureRect, const FExtent2D& textureSize, const Vector2& pivot, const Color& blendColor, const Matrix4x4& matWorld, std::vector<float>* vertices)
{
    const float leftUV = textureRect.x / textureSize.width;
    const float rightUV = (textureRect.x + textureRect.width) / textureSize.width;
#if TGON_GRAPHICS_OPENGL
    const float topUV = textureRect.y / textureSize.height;
    const float bottomUV = (textureRect.y + textureRect.height) / textureSize.height;
#else
    const float topUV = (textureRect.y + textureRect.height) / textureSize.height;
    const float bottomUV = textureRect.y / textureSize.height;
#endif
    const float halfWidth = textureSize.width * 0.5f;
    const float halfHeight = textureSize.height * 0.5f;
    const float xOffset = -textureSize.width * (pivot.x - 0.5f);
    const float yOffset = textureSize.height * (pivot.y - 0.5f);

    const int32_t oldVertexEndOffset = m_vertexEndOffset;
    const int32_t expandSize = sizeof(V3F_C4F_T2F) / 4 * 6;
    vertices->resize(static_cast<size_t>(m_vertexEndOffset) + expandSize);
    m_vertexEndOffset += expandSize;

    // Left top
    auto* newVertices = reinterpret_cast<V3F_C4F_T2F*>(&(*vertices)[oldVertexEndOffset]);
    newVertices[0].position = Vector4(-halfWidth + xOffset, halfHeight + yOffset, 0.0f, 1.0f) * matWorld;
    newVertices[0].color = blendColor;
    newVertices[0].uv = {leftUV, topUV};
    
    // Right top
    newVertices[1].position = Vector4(halfWidth + xOffset, halfHeight + yOffset, 0.0f, 1.0f) * matWorld;
    newVertices[1].color = blendColor;
    newVertices[1].uv = {rightUV, topUV};
    
    // Right bottom
    newVertices[2].position = Vector4(halfWidth + xOffset, -halfHeight + yOffset, 0.0f, 1.0f) * matWorld;
    newVertices[2].color = blendColor;
    newVertices[2].uv = {rightUV, bottomUV};
    
    // Right bottom
    newVertices[3] = newVertices[2];

    // Left bottom
    newVertices[4].position = Vector4(-halfWidth + xOffset, -halfHeight + yOffset, 0.0f, 1.0f) * matWorld;
    newVertices[4].color = blendColor;
    newVertices[4].uv = {leftUV, bottomUV};

    // Left top
    newVertices[5] = newVertices[0];
}

}
