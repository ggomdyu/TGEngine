#include "RendererModule.h"

TG_NAMESPACE_BEGIN

RendererModule::RendererModule(void* nativeWindow, const VideoMode& videoMode) :
    _graphics(nativeWindow, videoMode)
{
}

void RendererModule::Update()
{
    _graphics.ClearColorDepthBuffer();
    _graphics.SwapBuffer();
}

Graphics& RendererModule::GetGraphics() noexcept
{
    return _graphics;
}

const Graphics& RendererModule::GetGraphics() const noexcept
{
    return _graphics;
}

}
