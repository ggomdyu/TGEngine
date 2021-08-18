#pragma once

#if TG_GRAPHICS_OPENGL
#include "OpenGL/OpenGLRenderTarget.h"
#endif

#include "Texture.h"

TG_NAMESPACE_BEGIN

class RenderTarget final :
    private PlatformRenderTarget
{
public:
    explicit RenderTarget(const FExtent2D& extent, int32_t depthBits = 0, int32_t stencilBits = 0);

public:
    void Use();
    void Disuse();
    std::shared_ptr<Texture> GetTexture() noexcept;
    std::shared_ptr<const Texture> GetTexture() const noexcept;

private:
    std::shared_ptr<Texture> _texture;
};

}
