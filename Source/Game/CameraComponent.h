#pragma once

#include "Graphics/RenderTarget.h"
#include "Math/Matrix4x4.h"
#include "Math/Rect.h"

namespace tg
{
    
enum class ProjectionMode
{
    Perspective,
    Orthographic
};

class CameraComponent final
{
/**@section Constructor */
private:
    CameraComponent(ProjectionMode projectionMode, float nearClipPlane, float farClipPlane);

public:
    CameraComponent(const Vector3& eyePt, const Vector3& lookAt, float fieldOfView, float nearClipPlane, float farClipPlane) noexcept;
    CameraComponent(const Rect& pixelRect, float nearClipPlane, float farClipPlane) noexcept;

/**@section Method */
public:
    void Update();
    void SetFov(float fov) noexcept;
    void SetEyePt(const Vector3& eyePt) noexcept;
    void SetLookAt(const Vector3& lookAt) noexcept;
    void SetPixelRect(const Rect& pixelRect) noexcept;
    void SetNearClipPlane(float nearClipPlane) noexcept;
    void SetFarClipPlane(float farClipPlane) noexcept;
    void SetTargetTexture(const std::shared_ptr<RenderTarget>& renderTarget);
    [[nodiscard]] float GetAspect() const noexcept;
    [[nodiscard]] float GetFieldOfView() const noexcept;
    [[nodiscard]] const Vector3& GetEyePt() const noexcept;
    [[nodiscard]] const Vector3& GetLookAt() const noexcept;
    [[nodiscard]] float GetNearClipPlane() const noexcept;
    [[nodiscard]] float GetFarClipPlane() const noexcept;
    [[nodiscard]] const Rect& GetPixelRect() const noexcept;
    [[nodiscard]] std::shared_ptr<RenderTarget> GetTargetTexture() noexcept;
    [[nodiscard]] std::shared_ptr<const RenderTarget> GetTargetTexture() const noexcept;
    [[nodiscard]] ProjectionMode GetProjectionMode() const noexcept;
    [[nodiscard]] const Matrix4x4& GetProjectionMatrix() const noexcept;
    [[nodiscard]] const Matrix4x4& GetViewProjectionMatrix() const noexcept;

/**@section Variable */
protected:
    ProjectionMode m_projectionMode;
    Vector3 m_eyePt;
    Vector3 m_lookAt;
    float m_fieldOfView;
    float m_nearClipPlane;
    float m_farClipPlane;
    Rect m_pixelRect;
    Matrix4x4 m_matProj;
    Matrix4x4 m_matViewProj;
    bool m_isDirty;
    std::shared_ptr<RenderTarget> m_renderTarget;
};

}