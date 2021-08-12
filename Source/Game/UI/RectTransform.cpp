#include "PrecompiledHeader.h"

#include "RectTransform.h"

namespace tg
{

void RectTransform::SetLocalPivot(const Vector2& localPivot) noexcept
{
    m_localPivot = localPivot;
    m_isDirty = true;
}

const Vector2& RectTransform::GetLocalPivot() const noexcept
{
    return m_localPivot;
}

void RectTransform::SetSize(const FExtent2D& size) noexcept
{
    m_size = size;
    m_isDirty = true;
}

void RectTransform::SetWidth(float width) noexcept
{
    m_size.width = width;
    m_isDirty = true;
}

void RectTransform::SetHeight(float height) noexcept
{
    m_size.height = height;
    m_isDirty = true;
}

const FExtent2D& RectTransform::GetSize() const noexcept
{
    return m_size;
}

float RectTransform::GetWidth() const noexcept
{
    return m_size.width;
}

float RectTransform::GetHeight() const noexcept
{
    return m_size.height;
}

}
