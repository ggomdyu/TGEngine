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

TG_NAMESPACE_BEGIN

template <typename _KeyType>
class BasicTextureAtlas :
    private NonCopyable
{
private:
    enum : int32_t { DefaultPaddingOffset = 2 };

public:
    BasicTextureAtlas(const I32Extent2D& atlasSize, PixelFormat atlasPixelFormat, int32_t paddingOffset = DefaultPaddingOffset);
    BasicTextureAtlas(BasicTextureAtlas&& rhs) noexcept;

public:
    BasicTextureAtlas& operator=(BasicTextureAtlas&& rhs) noexcept;

public:
    void Initialize(const I32Extent2D& atlasSize, PixelFormat atlasPixelFormat, int32_t paddingOffset = DefaultPaddingOffset);
    bool Insert(const _KeyType& key, const std::byte* imageData, const I32Extent2D& size);
    bool Contains(const _KeyType& key);
    std::optional<Rect> GetTextureRect(const _KeyType& key) const;
    int32_t GetTextureCount() const noexcept;
    int32_t GetPaddingOffset() const noexcept;
    std::shared_ptr<const Texture> GetAtlasTexture() const noexcept;
    std::shared_ptr<Texture> GetAtlasTexture() noexcept;

private:
    std::shared_ptr<Texture> _atlasTexture;
    stbrp_context _context {};
    std::array<stbrp_node, 4096> _nodes {};
    std::array<stbrp_rect, 4096> _nodeRects {};
    mutable std::unordered_map<_KeyType, Rect> _packedTextureInfos;
    int32_t _paddingOffset = 0;
};

using TextureAtlas = BasicTextureAtlas<size_t>;
using I32TextureAtlas = BasicTextureAtlas<int32_t>;
using I64extureAtlas = BasicTextureAtlas<int64_t>;

template <typename _KeyType>
inline BasicTextureAtlas<_KeyType>::BasicTextureAtlas(const I32Extent2D& atlasSize, PixelFormat atlasPixelFormat, int32_t paddingOffset) :
    _atlasTexture(std::make_shared<Texture>(nullptr, atlasSize, atlasPixelFormat, FilterMode::Linear, WrapMode::Clamp, false, true)),
    _paddingOffset(paddingOffset)
{
    stbrp_init_target(&_context, static_cast<int>(atlasSize.width), static_cast<int>(atlasSize.height), &_nodes[0], 4096);
}

template <typename _KeyType>
inline BasicTextureAtlas<_KeyType>::BasicTextureAtlas(BasicTextureAtlas&& rhs) noexcept :
    _atlasTexture(std::move(rhs._atlasTexture)),
    _context(rhs._context),
    _nodes(rhs._nodes),
    _nodeRects(rhs._nodeRects),
    _paddingOffset(rhs._paddingOffset)
{
    rhs._paddingOffset = 0;
}

template <typename _KeyType>
inline BasicTextureAtlas<_KeyType>& BasicTextureAtlas<_KeyType>::operator=(BasicTextureAtlas&& rhs) noexcept
{
    _atlasTexture = std::move(rhs._atlasTexture);
    _context = rhs._context;
    _nodes = rhs._nodes;
    _nodeRects = rhs._nodeRects;
    _paddingOffset = rhs._paddingOffset;
    
    rhs._paddingOffset = 0;

    return *this;
}

template<typename _KeyType>
inline void BasicTextureAtlas<_KeyType>::Initialize(const I32Extent2D& atlasSize, PixelFormat atlasPixelFormat, int32_t paddingOffset)
{
    if ((_atlasTexture == nullptr) || (_atlasTexture->GetSize() != atlasSize) || (_atlasTexture->GetPixelFormat() != atlasPixelFormat))
    {
        _atlasTexture = std::make_shared<Texture>(nullptr, atlasSize, atlasPixelFormat, FilterMode::Linear, WrapMode::Clamp, false, true);
    }

    _context = {};
    _nodes = {};
    _nodeRects = {};
    _packedTextureInfos.clear();
    _paddingOffset = paddingOffset;

    stbrp_init_target(&_context, static_cast<int>(atlasSize.width), static_cast<int>(atlasSize.height), &_nodes[0], 4096);
}

template <typename _KeyType>
inline bool BasicTextureAtlas<_KeyType>::Insert(const _KeyType& key, const std::byte* imageData, const I32Extent2D& size)
{
    stbrp_rect rect{static_cast<int>(_packedTextureInfos.size()), static_cast<stbrp_coord>(size.width + _paddingOffset), static_cast<stbrp_coord>(size.height + _paddingOffset), 0, 0, 0};

    bool isPackingSucceed = stbrp_pack_rects(&_context, &rect, 1) == 1;
    if (isPackingSucceed)
    {
        _atlasTexture->SetData(imageData, Vector2(rect.x, rect.y), size, _atlasTexture->GetPixelFormat());
        _packedTextureInfos.emplace(key, Rect(static_cast<float>(rect.x), static_cast<float>(rect.y), static_cast<float>(rect.w - _paddingOffset), static_cast<float>(rect.h - _paddingOffset)));
    }

    return isPackingSucceed;
}

template <typename _KeyType>
inline bool BasicTextureAtlas<_KeyType>::Contains(const _KeyType& key)
{
    return _packedTextureInfos.find(key) != _packedTextureInfos.end();
}

template <typename _KeyType>
inline std::optional<Rect> BasicTextureAtlas<_KeyType>::GetTextureRect(const _KeyType& key) const
{
    auto it = _packedTextureInfos.find(key);
    if (it == _packedTextureInfos.end())
    {
        return {};
    }
    
    return it->second;
}

template <typename _KeyType>
inline int32_t BasicTextureAtlas<_KeyType>::GetTextureCount() const noexcept
{
    return static_cast<int32_t>(_packedTextureInfos.size());
}

template <typename _KeyType>
inline int32_t BasicTextureAtlas<_KeyType>::GetPaddingOffset() const noexcept
{
    return _paddingOffset;
}

template <typename _KeyType>
inline std::shared_ptr<const Texture> BasicTextureAtlas<_KeyType>::GetAtlasTexture() const noexcept
{
    return _atlasTexture;
}

template <typename _KeyType>
inline std::shared_ptr<Texture> BasicTextureAtlas<_KeyType>::GetAtlasTexture() noexcept
{
    return _atlasTexture;
}

}
