#include "TGEngine.Core/Mathf.h"
#include "TGEngine.World/Transform.h"

TG_NAMESPACE_BEGIN

void Transform::SetLocalPosition(const Vector3& localPosition) noexcept
{
    _localPosition = localPosition;
    _isDirty = true;
}

void Transform::SetLocalRotation(const Vector3& localRotation) noexcept
{
    _localRotation = localRotation;
    _isDirty = true;
}

void Transform::SetLocalScale(const Vector3& localScale) noexcept
{
    _localScale = localScale;
    _isDirty = true;
}

void Transform::SetParent(Transform* parent) noexcept
{
    _parent = parent;
    _isDirty = true;
}

const Vector3& Transform::GetLocalPosition() const noexcept
{
    return _localPosition;
}

const Vector3& Transform::GetLocalRotation() const noexcept
{
    return _localRotation;
}

const Vector3& Transform::GetLocalScale() const noexcept
{
    return _localScale;
}

const Matrix4x4& Transform::GetWorldMatrix() const noexcept
{
    if (_isDirty)
    {
        this->UpdateWorldMatrix();
        _isDirty = false;
    }

    return _matWorld;
}

Transform* Transform::GetParent() noexcept
{
    return _parent;
}

const Transform* Transform::GetParent() const noexcept
{
    return _parent;
}

void Transform::Update()
{
    if (_isDirty)
    {
        this->UpdateWorldMatrix();
        _isDirty = false;
    }
}

void Transform::UpdateWorldMatrix() const
{
    _matWorld = Matrix4x4::Scale(_localScale.x, _localScale.y, _localScale.z);
    _matWorld *= Matrix4x4::Rotate(_localRotation.x * Mathf::Deg2Rad, _localRotation.y * Mathf::Deg2Rad, _localRotation.z * Mathf::Deg2Rad);
    _matWorld *= Matrix4x4::Translate(_localPosition.x, _localPosition.y, _localPosition.z);

    if (auto owner = this->GetGameObject().lock(); owner != nullptr)
    {
        if (auto parent = owner->GetParent().lock(); parent != nullptr)
        {
            _matWorld *= parent->GetTransform()->GetWorldMatrix();
        }

        for (auto& child : owner->GetChildren())
        {
            child->GetTransform()->_isDirty = true;
        }
    }
}

TG_NAMESPACE_END