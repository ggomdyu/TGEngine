#pragma once

#include <vector>

#include "Core/RuntimeObject.h"
#include "Text/StringHash.h"

#include "Component.h"

namespace tg
{

class GameObject final :
    public RuntimeObject
{
public:
    TGON_RTTI(GameObject)

/**@section Constructor */
public:
    explicit GameObject(StringHash name = {});

/**@section Method */
public:
    /**
     * @brief   Updates the frame of the object.
     */
    void Update();

    /**
     * @brief   Adds a component to the object.
     * @param args  Arguments for construct the specified type of component.
     * @return  The added component.
     */
    template <typename _Component, typename... _Types>
    _Component* AddComponent(_Types&&... args);

    /**
     * @brief   Finds the component of the specified type.
     * @return  The found component or nullptr.
     */
    template <typename _Component>
    [[nodiscard]] _Component* FindComponent();

    /**
     * @brief   Finds the component of the specified type.
     * @return  The found component or nullptr.
     */
    template <typename _Component>
    [[nodiscard]] const _Component* FindComponent() const;

    /**
     * @brief   Sets the name of the object.
     * @param name  The object name.
     */
    void SetName(StringHash name);

    /**
     * @brief   Activates or deactivates the object.
     * @param isActive  True activates the object and false deactivates the object.
     */
    void SetActive(bool isActive) noexcept;

    /**
     * @brief   Gets the name of the object.
     * @return  The object name.
     */
    [[nodiscard]] const StringHash& GetName() const noexcept;

    /**
     * @brief   Gets the active state of the object.
     * @return  The active state of the object.
     */
    [[nodiscard]] bool IsActive() const noexcept;

/**@section Variable */
protected:
    StringHash m_name;
    bool m_isActive = true;
    GameObject* m_parent = nullptr;
    std::vector<std::unique_ptr<Component>> m_components;
};

template <typename _Component, typename... _Types>
_Component* GameObject::AddComponent(_Types&&... args)
{
    auto component = std::make_unique<_Component>(std::forward<_Types>(args)...);
    auto rawComponent = component.get();
    component->SetGameObject(this);
    component->Initialize();

    m_components.push_back(std::move(component));

    return rawComponent;
}

template <typename _Component>
_Component* GameObject::FindComponent()
{
    auto componentId = tg::GetRtti<_Component>()->GetHashCode();
    for (auto& component : m_components)
    {
        if (component->GetRtti()->GetHashCode() == componentId)
        {
            return component.get();
        }
    }

    return nullptr;
}

template <typename _Component>
const _Component* GameObject::FindComponent() const
{
    return const_cast<GameObject*>(this)->FindComponent<_Component>();
}

}
