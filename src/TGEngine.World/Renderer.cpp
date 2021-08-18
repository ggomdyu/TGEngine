#include "RendererComponent.h"

TG_NAMESPACE_BEGIN
    
RendererComponent::RendererComponent(const std::shared_ptr<Material>& material) noexcept :
    _material(material)
{
}

void RendererComponent::SetMaterial(const std::shared_ptr<Material>& material)
{
    _material = material;
}

std::shared_ptr<Material> RendererComponent::GetMaterial() noexcept
{
    return _material;
}

std::shared_ptr<const Material> RendererComponent::GetMaterial() const noexcept
{
    return _material;
}

}
