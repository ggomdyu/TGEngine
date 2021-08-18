#include "RectTransform.h"

TG_NAMESPACE_BEGIN

void RectTransform::SetLocalPivot(const Vector2& localPivot) noexcept
{
    _localPivot = localPivot;
    _isDirty = true;
}

const Vector2& RectTransform::GetLocalPivot() const noexcept
{
    return _localPivot;
}

void RectTransform::SetSize(const FExtent2D& size) noexcept
{
    _size = size;
    _isDirty = true;
}

void RectTransform::SetWidth(float width) noexcept
{
    _size.width = width;
    _isDirty = true;
}

void RectTransform::SetHeight(float height) noexcept
{
    _size.height = height;
    _isDirty = true;
}

const FExtent2D& RectTransform::GetSize() const noexcept
{
    return _size;
}

float RectTransform::GetWidth() const noexcept
{
    return _size.width;
}

float RectTransform::GetHeight() const noexcept
{
    return _size.height;
}

}
