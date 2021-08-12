#include "PrecompiledHeader.h"

#include "GameObject.h"
#include "Component.h"

namespace tg
{

GameObject::GameObject(StringHash name) :
    m_name(std::move(name))
{
}

void GameObject::Update()
{
    for (auto& component : m_components)
    {
        if (component->IsActive())
        {
            component->Update();
        }
    }
}

void GameObject::SetName(StringHash name)
{
    m_name = std::move(name);
}

void GameObject::SetActive(bool isActive) noexcept
{
    m_isActive = isActive;
}

bool GameObject::IsActive() const noexcept
{
    return m_isActive;
}

const StringHash& GameObject::GetName() const noexcept
{
    return m_name;
}

}
