/**
 * @file    CameraComponent.h
 * @author  ggomdyu
 * @since   08/11/2018
 */

#pragma once
#include "Graphics/Camera.h"

#include "Component.h"

namespace tgon
{

class CameraComponent :
    public Component
{
public:
    TGON_DECLARE_RTTI(CameraComponent)

/**@section Constructor */
public:
    CameraComponent();
    CameraComponent(const FRect& orthoPlane, float nearZ, float farZ);
    CameraComponent(const Vector3& eyePt, const Vector3& lookAt, float fov, float nearZ, float farZ);
    explicit CameraComponent(const std::shared_ptr<Camera>& camera);

/**@section Destructor */
public:
    ~CameraComponent() override;

/**@section Method */
public:
    void Update() override;
    void SetNearZ(float nearZ) noexcept;
    void SetFarZ(float farZ) noexcept;
    void SetCamera(const std::shared_ptr<Camera>& camera);
    float GetNearZ() const noexcept;
    float GetFarZ() const noexcept;
    ProjectionMode GetProjectionMode() const noexcept;
    const Matrix4x4& GetProjectionMatrix() const noexcept;
    const Matrix4x4& GetViewProjectionMatrix() const noexcept;
    std::shared_ptr<Camera> GetCamera() noexcept;
    std::shared_ptr<const Camera> GetCamera() const noexcept;

/**@section Variable */
private:
    std::shared_ptr<Camera> m_camera;
};

} /* namespace tgon */
