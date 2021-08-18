#pragma once

#include "TGEngine.Core/RuntimeObject.h"

TG_NAMESPACE_BEGIN

class Component :
    public RuntimeObject
{
public:
    TG_OBJECT(Component)

public:
    virtual void Initialize() {}
    virtual void Update() {}
    void SetGameObject(class GameObject* gameObject) noexcept;
    void SetActive(bool isActive) noexcept;
    GameObject* GetGameObject() noexcept;
    const GameObject* GetGameObject() const noexcept;
    bool IsActive() const noexcept;

protected:
    GameObject* _gameObject = nullptr;
    bool _isActive = true;
};

TG_NAMESPACE_END