#include "CameraComponent.h"

TG_NAMESPACE_BEGIN

CameraComponent::CameraComponent(const Vector3& eyePt, const Vector3& lookAt, float fieldOfView, float nearClipPlane, float farClipPlane) noexcept :
    _projectionMode(ProjectionMode::Perspective),
    _eyePt(eyePt),
    _lookAt(lookAt),
    _fieldOfView(fieldOfView),
    _nearClipPlane(nearClipPlane),
    _farClipPlane(farClipPlane),
    _isDirty(true)
{
    this->Update();
}

CameraComponent::CameraComponent(const Rect& pixelRect, float nearClipPlane, float farClipPlane) noexcept :
    _projectionMode(ProjectionMode::Orthographic),
    _nearClipPlane(nearClipPlane),
    _farClipPlane(farClipPlane),
    _pixelRect(pixelRect),
    _isDirty(true)
{
    this->Update();
}

void CameraComponent::SetNearClipPlane(float nearClipPlane) noexcept
{
    _nearClipPlane = nearClipPlane;
    _isDirty = true;
}

void CameraComponent::SetFarClipPlane(float farClipPlane) noexcept
{
    _farClipPlane = farClipPlane;
    _isDirty = true;
}

void CameraComponent::SetTargetTexture(const std::shared_ptr<RenderTarget>& renderTarget)
{
    _renderTarget = renderTarget;
}

float CameraComponent::GetAspect() const noexcept
{
    return _pixelRect.width / _pixelRect.height;
}

float CameraComponent::GetNearClipPlane() const noexcept
{
    return _nearClipPlane;
}

float CameraComponent::GetFarClipPlane() const noexcept
{
    return _farClipPlane;
}

std::shared_ptr<RenderTarget> CameraComponent::GetTargetTexture() noexcept
{
    return _renderTarget;
}

std::shared_ptr<const RenderTarget> CameraComponent::GetTargetTexture() const noexcept
{
    return _renderTarget;
}

ProjectionMode CameraComponent::GetProjectionMode() const noexcept
{
    return _projectionMode;
}

const Matrix4x4& CameraComponent::GetProjectionMatrix() const noexcept
{
    return _matProj;
}

const Matrix4x4& CameraComponent::GetViewProjectionMatrix() const noexcept
{
    return _matViewProj;
}

void CameraComponent::SetEyePt(const Vector3& eyePt) noexcept
{
    _eyePt = eyePt;
    _isDirty = true;
}

void CameraComponent::SetLookAt(const Vector3& lookAt) noexcept
{
    _lookAt = lookAt;
    _isDirty = true;
}

void CameraComponent::SetFov(float fov) noexcept
{
    _fieldOfView = fov;
    _isDirty = true;
}

const Vector3& CameraComponent::GetEyePt() const noexcept
{
    return _eyePt;
}

const Vector3& CameraComponent::GetLookAt() const noexcept
{
    return _lookAt;
}

float CameraComponent::GetFieldOfView() const noexcept
{
    return _fieldOfView;
}

void CameraComponent::Update()
{
    if (_isDirty == true)
    {
        if (_projectionMode == ProjectionMode::Perspective)
        {
            _matProj = Matrix4x4::PerspectiveRH(_fieldOfView, 1, _nearClipPlane, _farClipPlane);
            _matViewProj = Matrix4x4::LookAtRH(_eyePt, _lookAt, {0.0f, 1.0f, 0.0f});
        }
        else
        {
            _matProj = Matrix4x4::OrthographicRH(_pixelRect.x, _pixelRect.x + _pixelRect.width, _pixelRect.y, _pixelRect.y + _pixelRect.height, _nearClipPlane, _farClipPlane);
        }

        _matViewProj *= _matProj;
        
        _isDirty = false;
    }
}

void CameraComponent::SetPixelRect(const Rect& pixelRect) noexcept
{
    _pixelRect = pixelRect;
    _isDirty = true;
}

const Rect& CameraComponent::GetPixelRect() const noexcept
{
    return _pixelRect;
}

}
