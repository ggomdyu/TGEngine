#pragma once

#include "Math/Vector2.h"
#include "Math/Extent.h"
#include "Game/Transform.h"

TG_NAMESPACE_BEGIN

class RectTransform :
    public Transform
{
public:
    using Transform::Transform;

public:
    void SetLocalPivot(const Vector2& localPivot) noexcept;
    void SetSize(const FExtent2D& size) noexcept;
    void SetWidth(float width) noexcept;
    void SetHeight(float height) noexcept;
    const Vector2& GetLocalPivot() const noexcept;
    const FExtent2D& GetSize() const noexcept;
    float GetWidth() const noexcept;
    float GetHeight() const noexcept;
    void Update() override;

protected:
    Vector2 _localPivot;
    FExtent2D _size;
};

}
