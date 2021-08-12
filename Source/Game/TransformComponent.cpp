#include "PrecompiledHeader.h"

#include "Math/Mathf.h"
#include "Game/GameObject.h"

#include "TransformComponent.h"

namespace tg
{

void TransformComponent::SetLocalPosition(const Vector3& localPosition) noexcept
{
    m_localPosition = localPosition;
    m_isDirty = true;
}

void TransformComponent::SetLocalRotation(const Vector3& localRotation) noexcept
{
    m_localRotation = localRotation;
    m_isDirty = true;
}

void TransformComponent::SetLocalScale(const Vector3& localScale) noexcept
{
    m_localScale = localScale;
    m_isDirty = true;
}

const Vector3& TransformComponent::GetLocalPosition() const noexcept
{
    return m_localPosition;
}

const Vector3& TransformComponent::GetLocalRotation() const noexcept
{
    return m_localRotation;
}

const Vector3& TransformComponent::GetLocalScale() const noexcept
{
    return m_localScale;
}

const Matrix4x4& TransformComponent::GetWorldMatrix() const noexcept
{
    if (m_isDirty)
    {
        this->UpdateWorldMatrix();
        m_isDirty = false;
    }

    return m_matWorld;
}

void TransformComponent::Update()
{
    if (m_isDirty)
    {
        this->UpdateWorldMatrix();
        m_isDirty = false;
    }
}

void TransformComponent::UpdateWorldMatrix() const
{
    m_matWorld = Matrix4x4::Scale(m_localScale.x, m_localScale.y, m_localScale.z);
    m_matWorld *= Matrix4x4::Rotate(m_localRotation.x * Mathf::Deg2Rad, m_localRotation.y * Mathf::Deg2Rad, m_localRotation.z * Mathf::Deg2Rad);
    m_matWorld *= Matrix4x4::Translate(m_localPosition.x, m_localPosition.y, m_localPosition.z);

   /* if (auto owner = this->GetGameObject().lock(); owner != nullptr)
    {
        if (auto parent = owner->GetParent().lock(); parent != nullptr)
        {
            m_matWorld *= parent->GetTransform()->GetWorldMatrix();
        }

        for (auto& child : owner->GetChildren())
        {
            child->GetTransform()->m_isDirty = true;
        }
    }*/
}

}
