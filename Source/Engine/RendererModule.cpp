#include "PrecompiledHeader.h"

#include "RendererModule.h"

namespace tg
{

RendererModule::RendererModule(void* nativeWindow, const VideoMode& videoMode) :
    m_graphics(nativeWindow, videoMode)
{
}

void RendererModule::Update()
{
    m_graphics.ClearColorDepthBuffer();
    m_graphics.SwapBuffer();
}

Graphics& RendererModule::GetGraphics() noexcept
{
    return m_graphics;
}

const Graphics& RendererModule::GetGraphics() const noexcept
{
    return m_graphics;
}

}
