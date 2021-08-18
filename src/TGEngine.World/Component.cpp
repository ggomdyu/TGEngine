#include "Component.h"

TG_NAMESPACE_BEGIN

void Component::SetGameObject(GameObject* gameObject) noexcept
{
    _gameObject = gameObject;
}

GameObject* Component::GetGameObject() noexcept
{
    return _gameObject;
}

const GameObject* Component::GetGameObject() const noexcept
{
    return _gameObject;
}

void Component::SetActive(bool isActive) noexcept
{
    _isActive = isActive;
}

bool Component::IsActive() const noexcept
{
    return _isActive;
}

}
