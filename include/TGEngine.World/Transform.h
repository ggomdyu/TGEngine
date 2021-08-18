#pragma once

#include "TGEngine.Core/Matrix4x4.h"
#include "TGEngine.World/Component.h"

TG_NAMESPACE_BEGIN

class Transform :
    public Component
{
public:
    TG_OBJECT(Transform)

public:
    using Component::Component;

public:
    void SetLocalPosition(const Vector3& localPosition) noexcept;
    void SetLocalRotation(const Vector3& localRotation) noexcept;
    void SetLocalScale(const Vector3& localScale) noexcept;
    void SetParent(Transform* parent) noexcept;
    const Vector3& GetLocalPosition() const noexcept;
    const Vector3& GetLocalRotation() const noexcept;
    const Vector3& GetLocalScale() const noexcept;
    const Matrix4x4& GetWorldMatrix() const noexcept;
    Transform* GetParent() noexcept;
    const Transform* GetParent() const noexcept;
    void Update() override;

private:
    void UpdateWorldMatrix() const;

protected:
    Vector3 _localPosition;
    Vector3 _localRotation;
    Vector3 _localScale = Vector3(1.0f, 1.0f, 1.0f);
    mutable Matrix4x4 _matWorld;
    mutable bool _isDirty = true;
};

TG_NAMESPACE_END