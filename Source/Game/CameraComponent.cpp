#include "PrecompiledHeader.h"

#include "CameraComponent.h"

namespace tg
{

CameraComponent::CameraComponent(const Vector3& eyePt, const Vector3& lookAt, float fieldOfView, float nearClipPlane, float farClipPlane) noexcept :
    m_projectionMode(ProjectionMode::Perspective),
    m_eyePt(eyePt),
    m_lookAt(lookAt),
    m_fieldOfView(fieldOfView),
    m_nearClipPlane(nearClipPlane),
    m_farClipPlane(farClipPlane),
    m_isDirty(true)
{
    this->Update();
}

CameraComponent::CameraComponent(const Rect& pixelRect, float nearClipPlane, float farClipPlane) noexcept :
    m_projectionMode(ProjectionMode::Orthographic),
    m_nearClipPlane(nearClipPlane),
    m_farClipPlane(farClipPlane),
    m_pixelRect(pixelRect),
    m_isDirty(true)
{
    this->Update();
}

void CameraComponent::SetNearClipPlane(float nearClipPlane) noexcept
{
    m_nearClipPlane = nearClipPlane;
    m_isDirty = true;
}

void CameraComponent::SetFarClipPlane(float farClipPlane) noexcept
{
    m_farClipPlane = farClipPlane;
    m_isDirty = true;
}

void CameraComponent::SetTargetTexture(const std::shared_ptr<RenderTarget>& renderTarget)
{
    m_renderTarget = renderTarget;
}

float CameraComponent::GetAspect() const noexcept
{
    return m_pixelRect.width / m_pixelRect.height;
}

float CameraComponent::GetNearClipPlane() const noexcept
{
    return m_nearClipPlane;
}

float CameraComponent::GetFarClipPlane() const noexcept
{
    return m_farClipPlane;
}

std::shared_ptr<RenderTarget> CameraComponent::GetTargetTexture() noexcept
{
    return m_renderTarget;
}

std::shared_ptr<const RenderTarget> CameraComponent::GetTargetTexture() const noexcept
{
    return m_renderTarget;
}

ProjectionMode CameraComponent::GetProjectionMode() const noexcept
{
    return m_projectionMode;
}

const Matrix4x4& CameraComponent::GetProjectionMatrix() const noexcept
{
    return m_matProj;
}

const Matrix4x4& CameraComponent::GetViewProjectionMatrix() const noexcept
{
    return m_matViewProj;
}

void CameraComponent::SetEyePt(const Vector3& eyePt) noexcept
{
    m_eyePt = eyePt;
    m_isDirty = true;
}

void CameraComponent::SetLookAt(const Vector3& lookAt) noexcept
{
    m_lookAt = lookAt;
    m_isDirty = true;
}

void CameraComponent::SetFov(float fov) noexcept
{
    m_fieldOfView = fov;
    m_isDirty = true;
}

const Vector3& CameraComponent::GetEyePt() const noexcept
{
    return m_eyePt;
}

const Vector3& CameraComponent::GetLookAt() const noexcept
{
    return m_lookAt;
}

float CameraComponent::GetFieldOfView() const noexcept
{
    return m_fieldOfView;
}

void CameraComponent::Update()
{
    if (m_isDirty == true)
    {
        if (m_projectionMode == ProjectionMode::Perspective)
        {
            m_matProj = Matrix4x4::PerspectiveRH(m_fieldOfView, 1, m_nearClipPlane, m_farClipPlane);
            m_matViewProj = Matrix4x4::LookAtRH(m_eyePt, m_lookAt, {0.0f, 1.0f, 0.0f});
        }
        else
        {
            m_matProj = Matrix4x4::OrthographicRH(m_pixelRect.x, m_pixelRect.x + m_pixelRect.width, m_pixelRect.y, m_pixelRect.y + m_pixelRect.height, m_nearClipPlane, m_farClipPlane);
        }

        m_matViewProj *= m_matProj;
        
        m_isDirty = false;
    }
}

void CameraComponent::SetPixelRect(const Rect& pixelRect) noexcept
{
    m_pixelRect = pixelRect;
    m_isDirty = true;
}

const Rect& CameraComponent::GetPixelRect() const noexcept
{
    return m_pixelRect;
}

}
