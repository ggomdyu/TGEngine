#include "PrecompiledHeader.h"

#include "RenderTarget.h"

namespace tg
{

RenderTarget::RenderTarget(const FExtent2D& extent, int32_t depthBits, int32_t stencilBits) :
    PlatformRenderTarget(extent, depthBits, stencilBits)
{
}

std::shared_ptr<Texture> RenderTarget::GetTexture() noexcept
{
    return m_texture;
}

std::shared_ptr<const Texture> RenderTarget::GetTexture() const noexcept
{
    return m_texture;
}

}
