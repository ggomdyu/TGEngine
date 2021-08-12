#include "PrecompiledHeader.h"

#include "Component.h"

namespace tg
{

void Component::SetGameObject(GameObject* gameObject) noexcept
{
    m_gameObject = gameObject;
}

GameObject* Component::GetGameObject() noexcept
{
    return m_gameObject;
}

const GameObject* Component::GetGameObject() const noexcept
{
    return m_gameObject;
}

void Component::SetActive(bool isActive) noexcept
{
    m_isActive = isActive;
}

bool Component::IsActive() const noexcept
{
    return m_isActive;
}

}
