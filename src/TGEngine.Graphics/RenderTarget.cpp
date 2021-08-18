#include "RenderTarget.h"

TG_NAMESPACE_BEGIN

RenderTarget::RenderTarget(const FExtent2D& extent, int32_t depthBits, int32_t stencilBits) :
    PlatformRenderTarget(extent, depthBits, stencilBits)
{
}

std::shared_ptr<Texture> RenderTarget::GetTexture() noexcept
{
    return _texture;
}

std::shared_ptr<const Texture> RenderTarget::GetTexture() const noexcept
{
    return _texture;
}

}
