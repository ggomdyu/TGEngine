#include "PrecompiledHeader.h"

#include "Graphics.h"

namespace tg
{

Graphics::Graphics(void* nativeWindow, const VideoMode& videoMode) :
    PlatformGraphics(nativeWindow, videoMode),
    m_nativeWindow(nativeWindow)
{
    this->SetClearColor(videoMode.clearColor);
    this->SetCullMode(CullMode::Back);
    this->SetBlendMode(BlendMode::One, BlendMode::One);
    this->SetEnableZTest(true);
    this->SetEnableBlend(true);
}

PlatformGraphics& Graphics::GetPlatformDependency() noexcept
{
    return *this;
}

const PlatformGraphics& Graphics::GetPlatformDependency() const noexcept
{
    return *this;
}

}
