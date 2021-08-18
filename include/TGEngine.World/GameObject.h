#pragma once

#include <vector>

#include "TGEngine.Core/RuntimeObject.h"
#include "TGEngine.Core/StringHash.h"
#include "TGEngine.World/Component.h"
#include "TGEngine.World/Transform.h"

TG_NAMESPACE_BEGIN

class GameObject final :
    public RuntimeObject
{
public:
    TG_OBJECT(GameObject)

public:
    explicit GameObject(std::u16string_view name);

public:
    template <typename T, typename... Args>
    T* AddComponent(Args&&... args);
    template <typename T>
    T* FindComponent();
    template <typename T>
    const T* FindComponent() const;
    void SetName(StringHash name);
    void SetActive(bool isActive) noexcept;
    const StringHash& GetName() const noexcept;
    bool IsActive() const noexcept;
    void Update();

protected:
    StringHash _name;
    bool _isActive = true;
    std::vector<std::shared_ptr<Component>> _components;
};

template <typename T, typename... Args>
T* GameObject::AddComponent(Args&&... args)
{
    auto component = std::make_shared<T>(std::forward<Args>(args)...);
    auto rawComponent = component.get();
    component->SetGameObject(this);
    component->Initialize();

    _components.push_back(std::move(component));

    return rawComponent;
}

template <typename T>
T* GameObject::FindComponent()
{
    auto componentId = tg::GetRtti<T>()->GetHashCode();
    for (auto& component : _components)
    {
        if (component->GetRtti()->GetHashCode() == componentId)
        {
            return component.get();
        }
    }

    return nullptr;
}

template <typename T>
const T* GameObject::FindComponent() const
{
    return const_cast<GameObject*>(this)->FindComponent<T>();
}

TG_NAMESPACE_END