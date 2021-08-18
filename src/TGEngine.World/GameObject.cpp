#include "GameObject.h"
#include "Component.h"

TG_NAMESPACE_BEGIN

GameObject::GameObject(StringHash name) :
    _name(std::move(name))
{
}

void GameObject::Update()
{
    for (auto& component : _components)
    {
        if (component->IsActive())
        {
            component->Update();
        }
    }
}

void GameObject::SetName(StringHash name)
{
    _name = std::move(name);
}

void GameObject::SetActive(bool isActive) noexcept
{
    _isActive = isActive;
}

bool GameObject::IsActive() const noexcept
{
    return _isActive;
}

const StringHash& GameObject::GetName() const noexcept
{
    return _name;
}

}
