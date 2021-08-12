#pragma once

#include <memory>

#include "Math/Matrix4x4.h"

#include "Component.h"

namespace tg
{

class TransformComponent :
    public Component
{
public:
    TGON_RTTI(TransformComponent)

/**@section Constructor */
public:
    using Component::Component;

/**@section Method */
public:
    void SetLocalPosition(const Vector3& localPosition) noexcept;
    void SetLocalRotation(const Vector3& localRotation) noexcept;
    void SetLocalScale(const Vector3& localScale) noexcept;
    [[nodiscard]] const Vector3& GetLocalPosition() const noexcept;
    [[nodiscard]] const Vector3& GetLocalRotation() const noexcept;
    [[nodiscard]] const Vector3& GetLocalScale() const noexcept;
    [[nodiscard]] const Matrix4x4& GetWorldMatrix() const noexcept;
    void Update() override;

private:
    void UpdateWorldMatrix() const;

/**@section Variable */
protected:
    Vector3 m_localPosition;
    Vector3 m_localRotation;
    Vector3 m_localScale = Vector3(1.0f, 1.0f, 1.0f);
    mutable Matrix4x4 m_matWorld;
    mutable bool m_isDirty = true;
};

}

