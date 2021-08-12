#pragma once

#include "Core/RuntimeObject.h"

namespace tg
{

class GameObject;

class Component :
    public RuntimeObject
{
public:
    TGON_RTTI(Component)

/**@section Method */
public:
    virtual void Initialize() {}
    virtual void Update() {}
    void SetGameObject(GameObject* gameObject) noexcept;
    void SetActive(bool isActive) noexcept;
    [[nodiscard]] GameObject* GetGameObject() noexcept;
    [[nodiscard]] const GameObject* GetGameObject() const noexcept;
    [[nodiscard]] bool IsActive() const noexcept;

/**@section Variable */
protected:
    GameObject* m_gameObject{};
    bool m_isActive = true;
};

}
