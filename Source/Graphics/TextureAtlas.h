#pragma once

#include <unordered_map>
#include <optional>
#include <memory>
#include <array>
#include <stb_rect_pack.h>

#include "Core/NonCopyable.h"
#include "Math/Rect.h"
#include "Math/Extent.h"

#include "Texture.h"

namespace tg
{

template <typename _KeyType>
class BasicTextureAtlas :
    private NonCopyable
{
/**@section Enum */
private:
    enum : int32_t { DefaultPaddingOffset = 2 };

/**@section Constructor */
public:
    BasicTextureAtlas(const I32Extent2D& atlasSize, PixelFormat atlasPixelFormat, int32_t paddingOffset = DefaultPaddingOffset);
    BasicTextureAtlas(BasicTextureAtlas&& rhs) noexcept;

/**@section Operator */
public:
    BasicTextureAtlas& operator=(BasicTextureAtlas&& rhs) noexcept;

/**@section Method */
public:
    void Initialize(const I32Extent2D& atlasSize, PixelFormat atlasPixelFormat, int32_t paddingOffset = DefaultPaddingOffset);
    bool Insert(const _KeyType& key, const std::byte* imageData, const I32Extent2D& size);
    bool Contains(const _KeyType& key);
    std::optional<Rect> GetTextureRect(const _KeyType& key) const;
    int32_t GetTextureCount() const noexcept;
    int32_t GetPaddingOffset() const noexcept;
    std::shared_ptr<const Texture> GetAtlasTexture() const noexcept;
    std::shared_ptr<Texture> GetAtlasTexture() noexcept;

/**@section Variable */
private:
    std::shared_ptr<Texture> m_atlasTexture;
    stbrp_context m_context {};
    std::array<stbrp_node, 4096> m_nodes {};
    std::array<stbrp_rect, 4096> m_nodeRects {};
    mutable std::unordered_map<_KeyType, Rect> m_packedTextureInfos;
    int32_t m_paddingOffset = 0;
};

using TextureAtlas = BasicTextureAtlas<size_t>;
using I32TextureAtlas = BasicTextureAtlas<int32_t>;
using I64extureAtlas = BasicTextureAtlas<int64_t>;

template <typename _KeyType>
inline BasicTextureAtlas<_KeyType>::BasicTextureAtlas(const I32Extent2D& atlasSize, PixelFormat atlasPixelFormat, int32_t paddingOffset) :
    m_atlasTexture(std::make_shared<Texture>(nullptr, atlasSize, atlasPixelFormat, FilterMode::Linear, WrapMode::Clamp, false, true)),
    m_paddingOffset(paddingOffset)
{
    stbrp_init_target(&m_context, static_cast<int>(atlasSize.width), static_cast<int>(atlasSize.height), &m_nodes[0], 4096);
}

template <typename _KeyType>
inline BasicTextureAtlas<_KeyType>::BasicTextureAtlas(BasicTextureAtlas&& rhs) noexcept :
    m_atlasTexture(std::move(rhs.m_atlasTexture)),
    m_context(rhs.m_context),
    m_nodes(rhs.m_nodes),
    m_nodeRects(rhs.m_nodeRects),
    m_paddingOffset(rhs.m_paddingOffset)
{
    rhs.m_paddingOffset = 0;
}

template <typename _KeyType>
inline BasicTextureAtlas<_KeyType>& BasicTextureAtlas<_KeyType>::operator=(BasicTextureAtlas&& rhs) noexcept
{
    m_atlasTexture = std::move(rhs.m_atlasTexture);
    m_context = rhs.m_context;
    m_nodes = rhs.m_nodes;
    m_nodeRects = rhs.m_nodeRects;
    m_paddingOffset = rhs.m_paddingOffset;
    
    rhs.m_paddingOffset = 0;

    return *this;
}

template<typename _KeyType>
inline void BasicTextureAtlas<_KeyType>::Initialize(const I32Extent2D& atlasSize, PixelFormat atlasPixelFormat, int32_t paddingOffset)
{
    if ((m_atlasTexture == nullptr) || (m_atlasTexture->GetSize() != atlasSize) || (m_atlasTexture->GetPixelFormat() != atlasPixelFormat))
    {
        m_atlasTexture = std::make_shared<Texture>(nullptr, atlasSize, atlasPixelFormat, FilterMode::Linear, WrapMode::Clamp, false, true);
    }

    m_context = {};
    m_nodes = {};
    m_nodeRects = {};
    m_packedTextureInfos.clear();
    m_paddingOffset = paddingOffset;

    stbrp_init_target(&m_context, static_cast<int>(atlasSize.width), static_cast<int>(atlasSize.height), &m_nodes[0], 4096);
}

template <typename _KeyType>
inline bool BasicTextureAtlas<_KeyType>::Insert(const _KeyType& key, const std::byte* imageData, const I32Extent2D& size)
{
    stbrp_rect rect{static_cast<int>(m_packedTextureInfos.size()), static_cast<stbrp_coord>(size.width + m_paddingOffset), static_cast<stbrp_coord>(size.height + m_paddingOffset), 0, 0, 0};

    bool isPackingSucceed = stbrp_pack_rects(&m_context, &rect, 1) == 1;
    if (isPackingSucceed)
    {
        m_atlasTexture->SetData(imageData, Vector2(rect.x, rect.y), size, m_atlasTexture->GetPixelFormat());
        m_packedTextureInfos.emplace(key, Rect(static_cast<float>(rect.x), static_cast<float>(rect.y), static_cast<float>(rect.w - m_paddingOffset), static_cast<float>(rect.h - m_paddingOffset)));
    }

    return isPackingSucceed;
}

template <typename _KeyType>
inline bool BasicTextureAtlas<_KeyType>::Contains(const _KeyType& key)
{
    return m_packedTextureInfos.find(key) != m_packedTextureInfos.end();
}

template <typename _KeyType>
inline std::optional<Rect> BasicTextureAtlas<_KeyType>::GetTextureRect(const _KeyType& key) const
{
    auto it = m_packedTextureInfos.find(key);
    if (it == m_packedTextureInfos.end())
    {
        return {};
    }
    
    return it->second;
}

template <typename _KeyType>
inline int32_t BasicTextureAtlas<_KeyType>::GetTextureCount() const noexcept
{
    return static_cast<int32_t>(m_packedTextureInfos.size());
}

template <typename _KeyType>
inline int32_t BasicTextureAtlas<_KeyType>::GetPaddingOffset() const noexcept
{
    return m_paddingOffset;
}

template <typename _KeyType>
inline std::shared_ptr<const Texture> BasicTextureAtlas<_KeyType>::GetAtlasTexture() const noexcept
{
    return m_atlasTexture;
}

template <typename _KeyType>
inline std::shared_ptr<Texture> BasicTextureAtlas<_KeyType>::GetAtlasTexture() noexcept
{
    return m_atlasTexture;
}

}
