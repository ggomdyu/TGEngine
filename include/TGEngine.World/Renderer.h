#pragma once

#include "Graphics/Material.h"

#include "Component.h"

TG_NAMESPACE_BEGIN

class RendererComponent :
    public Component
{
public:
    TG_OBJECT(RendererComponent)

public:
    explicit RendererComponent(const std::shared_ptr<Material>& material = nullptr) noexcept;
    
public:
    void SetMaterial(const std::shared_ptr<Material>& material);
    std::shared_ptr<Material> GetMaterial() noexcept;
    std::shared_ptr<const Material> GetMaterial() const noexcept;
    
protected:
    std::shared_ptr<Material> _material;
};

}
