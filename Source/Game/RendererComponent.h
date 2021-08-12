#pragma once

#include "Graphics/Material.h"

#include "Component.h"

namespace tg
{

class RendererComponent :
    public Component
{
public:
    TGON_RTTI(RendererComponent)

/**@section Constructor */
public:
    explicit RendererComponent(const std::shared_ptr<Material>& material = nullptr) noexcept;
    
/**@section Method */
public:
    void SetMaterial(const std::shared_ptr<Material>& material);
    [[nodiscard]] std::shared_ptr<Material> GetMaterial() noexcept;
    [[nodiscard]] std::shared_ptr<const Material> GetMaterial() const noexcept;
    
/**@section Variable */
protected:
    std::shared_ptr<Material> m_material;
};

}
