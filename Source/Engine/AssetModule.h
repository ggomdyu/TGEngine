#pragma once

#include <mutex>
#include <any>
#include <unordered_map>

#include "Audio/AudioClip.h"
#include "Text/StringHash.h"
#include "Drawing/FontFactory.h"
#include "Graphics/Texture.h"
#include "Graphics/FontAtlas.h"

#include "Module.h"

namespace tg
{
//
//class AssetModule :
//	public Module
//{
//public:
//    TGON_RTTI(AssetModule)
//
///* @section Type */
//private:
//    using ResourceCache = std::unordered_map<U8StringHash, std::any>;
//    using ResourceUnitTable = std::vector<ResourceCache>;
//    using ResourceUnit = size_t;
//
///**@section Constructor */
//public:
//    AssetModule();
//
///* @section Method */
//public:
//    template <typename _Resource>
//    std::shared_ptr<_Resource> GetResource(const U8StringViewHash& path);
//    template <typename _Resource>
//    std::shared_ptr<const _Resource> GetResource(const U8StringViewHash& path) const;
//    template <typename _Resource>
//    void PurgeResource(const U8StringViewHash& path);
//    template <typename _Resource>
//    void PurgeResource();
//    void PurgeAllResource();
//
//private:
//    template <typename _Resource>
//    std::shared_ptr<_Resource> CreateResource(const U8StringViewHash& path);
//    template <typename _Resource>
//    ResourceUnit GetResourceUnit() const;
//
///* @section Variable */
//public:
//    static constexpr auto ModuleStage = ModuleStage::Idle;
//
//private:
//    std::mutex m_mutex;
//    FontFactory m_fontFactory;
//    mutable ResourceUnitTable m_resourceUnitTable;
//    inline static ResourceUnit m_maxResourceUnit;
//};
//
//template<typename _Resource>
//std::shared_ptr<_Resource> AssetModule::GetResource(const U8StringViewHash& path)
//{
//    std::lock_guard lock(m_mutex);
//
//    ResourceCache* resourceCache = &m_resourceUnitTable[GetResourceUnit<_Resource>()];
//    auto it = resourceCache->find(path);
//    if (it == resourceCache->end())
//    {
//        auto resource = this->CreateResource<_Resource>(path);
//
//        // resourceCache can be a dangling pointer because of reallocation of m_resourceUnitTable.
//        resourceCache = &m_resourceUnitTable[GetResourceUnit<_Resource>()];
//
//        it = resourceCache->emplace(path, std::move(resource)).first;
//    }
//
//    return std::any_cast<std::shared_ptr<_Resource>>(it->second);
//}
//
//template<typename _Resource>
//std::shared_ptr<const _Resource> AssetModule::GetResource(const U8StringViewHash& path)
//{
//    return const_cast<AssetModule*>(this)->GetResource<_Resource>(path);
//}
//
//template <typename _Resource>
//std::shared_ptr<_Resource> AssetModule::CreateResource(const U8StringViewHash& path)
//{
//    return _Resource::Create(path.Data());
//}
//
//template <>
//inline std::shared_ptr<Texture> AssetModule::CreateResource(const U8StringViewHash& path) const
//{
//    auto image = Image::FromFile(path.Data());
//    if (image.has_value() == false)
//    {
//        return nullptr;
//    }
//
//    return std::make_shared<Texture>(*image, FilterMode::Linear, WrapMode::Clamp, false, false);
//}
//
//template <>
//inline std::shared_ptr<Font> AssetModule::CreateResource(const U8StringViewHash& path)
//{
//    return Font::Create(nullptr, path.Data());
//}
//
//template <>
//inline std::shared_ptr<FontAtlas> AssetModule::CreateResource(const U8StringViewHash& path)
//{
//    ResourceCache& resourceCache = m_resourceUnitTable[GetResourceUnit<Font>()];
//    auto it = resourceCache.find(path);
//    if (it == resourceCache.end())
//    {
//        it = resourceCache.emplace(path, this->CreateResource<Font>(path)).first;
//    }
//
//    return std::make_shared<FontAtlas>(std::any_cast<std::shared_ptr<Font>>(it->second));
//}
//
//template<typename _Resource>
//AssetModule::ResourceUnit AssetModule::GetResourceUnit() const
//{
//    static ResourceUnit resourceUnit = [&]()
//    {
//        ResourceUnit ret = m_maxResourceUnit;
//        m_resourceUnitTable.resize(++m_maxResourceUnit);
//        return ret;
//    } ();
//
//    return resourceUnit;
//}
//
//template<typename _Resource>
//void AssetModule::PurgeResource(const U8StringViewHash& path)
//{
//    m_resourceUnitTable[GetResourceUnit<Font>()].erase(path);
//}
//
//template <typename _Resource>
//void AssetModule::PurgeResource()
//{
//    m_resourceUnitTable[GetResourceUnit<Font>()].clear();
//}
//
//inline void AssetModule::PurgeAllResource()
//{
//    m_resourceUnitTable.clear();
//}

}