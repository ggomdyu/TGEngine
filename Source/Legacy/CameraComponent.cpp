#include "PrecompiledHeader.h"

#include "Engine/UIRendererModule.h"
#include "Graphics/Camera.h"
#include "Platform/Application.h"

#include "CameraComponent.h"

namespace tgon
{

CameraComponent::CameraComponent() :
    CameraComponent(nullptr)
{
}

CameraComponent::CameraComponent(const FRect& orthoPlane, float nearZ, float farZ) :
    CameraComponent(std::make_shared<OrthographicCamera>(orthoPlane, nearZ, farZ))
{
}

CameraComponent::CameraComponent(const Vector3& eyePt, const Vector3& lookAt, float fov, float nearZ, float farZ) :
    CameraComponent(std::make_shared<PerspectiveCamera>(eyePt, lookAt, fov, nearZ, farZ))
{
}

CameraComponent::CameraComponent(const std::shared_ptr<Camera>& camera) :
    Component(),
    m_camera(camera)
{
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Update()
{
    if (m_camera != nullptr)
    {
        m_camera->Update();
    }
}

void CameraComponent::SetNearZ(float nearZ) noexcept
{
    return m_camera->SetNearZ(nearZ);
}

void CameraComponent::SetFarZ(float farZ) noexcept
{
    return m_camera->SetFarZ(farZ);
}

float CameraComponent::GetNearZ() const noexcept
{
    return m_camera->GetNearZ();
}

float CameraComponent::GetFarZ() const noexcept
{
    return m_camera->GetFarZ();
}

ProjectionMode CameraComponent::GetProjectionMode() const noexcept
{
    return m_camera->GetProjectionMode();
}

const Matrix4x4& CameraComponent::GetProjectionMatrix() const noexcept
{
    return m_camera->GetProjectionMatrix();
}

const Matrix4x4& CameraComponent::GetViewProjectionMatrix() const noexcept
{
    return m_camera->GetViewProjectionMatrix();
}

void CameraComponent::SetCamera(const std::shared_ptr<Camera>& camera)
{
    m_camera = camera;
}

std::shared_ptr<Camera> CameraComponent::GetCamera() noexcept
{
    return m_camera;
}

std::shared_ptr<const Camera> CameraComponent::GetCamera() const noexcept
{
    return m_camera;
}

} /* namespace tgon */
