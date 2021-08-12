#include "PrecompiledHeader.h"

#include "Graphics/OpenGL/OpenGLShaderCode.h"

#include "UIElement.h"

namespace tg
{

UIElement::UIElement()
{
    this->InitializeDefaultMaterial();
}

void UIElement::SetMaterial(std::shared_ptr<Material>&& material) noexcept
{
    m_material = std::move(material);
}

void UIElement::SetMaterial(const std::shared_ptr<Material>& material) noexcept
{
    m_material = material;
}

std::shared_ptr<Material> UIElement::GetMaterial() noexcept
{
    return m_material;
}

std::shared_ptr<const Material> UIElement::GetMaterial() const noexcept
{
    return m_material;
}

void UIElement::InitializeDefaultMaterial()
{
    static void* callOnce = [&]()
    {
        m_defaultMaterial = Material::Create(g_positionColorUVVert, g_positionColorUVFrag, ReturnPointerTag{});
        return nullptr;
    } ();
}

}