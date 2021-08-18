#pragma once

#include "Graphics/RenderTarget.h"
#include "Math/Matrix4x4.h"
#include "Math/Rect.h"

TG_NAMESPACE_BEGIN
    
enum class ProjectionMode
{
    Perspective,
    Orthographic
};

class CameraComponent final
{
private:
    CameraComponent(ProjectionMode projectionMode, float nearClipPlane, float farClipPlane);

public:
    CameraComponent(const Vector3& eyePt, const Vector3& lookAt, float fieldOfView, float nearClipPlane, float farClipPlane) noexcept;
    CameraComponent(const Rect& pixelRect, float nearClipPlane, float farClipPlane) noexcept;

public:
    void Update();
    void SetFov(float fov) noexcept;
    void SetEyePt(const Vector3& eyePt) noexcept;
    void SetLookAt(const Vector3& lookAt) noexcept;
    void SetPixelRect(const Rect& pixelRect) noexcept;
    void SetNearClipPlane(float nearClipPlane) noexcept;
    void SetFarClipPlane(float farClipPlane) noexcept;
    void SetTargetTexture(const std::shared_ptr<RenderTarget>& renderTarget);
    float GetAspect() const noexcept;
    float GetFieldOfView() const noexcept;
    const Vector3& GetEyePt() const noexcept;
    const Vector3& GetLookAt() const noexcept;
    float GetNearClipPlane() const noexcept;
    float GetFarClipPlane() const noexcept;
    const Rect& GetPixelRect() const noexcept;
    std::shared_ptr<RenderTarget> GetTargetTexture() noexcept;
    std::shared_ptr<const RenderTarget> GetTargetTexture() const noexcept;
    ProjectionMode GetProjectionMode() const noexcept;
    const Matrix4x4& GetProjectionMatrix() const noexcept;
    const Matrix4x4& GetViewProjectionMatrix() const noexcept;

protected:
    ProjectionMode _projectionMode;
    Vector3 _eyePt;
    Vector3 _lookAt;
    float _fieldOfView;
    float _nearClipPlane;
    float _farClipPlane;
    Rect _pixelRect;
    Matrix4x4 _matProj;
    Matrix4x4 _matViewProj;
    bool _isDirty;
    std::shared_ptr<RenderTarget> _renderTarget;
};

}