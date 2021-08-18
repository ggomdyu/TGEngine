#pragma once

#include "Graphics/Graphics.h"

#include "Module.h"

TG_NAMESPACE_BEGIN

class RendererModule final :
	public Module
{
public:
    TG_OBJECT(RendererModule)

public:
    RendererModule(void* nativeWindow, const VideoMode& videoMode);

public:
    void Update() override;
    Graphics& GetGraphics() noexcept;
    const Graphics& GetGraphics() const noexcept;

public:
    static constexpr auto ModuleStage = ModuleStage::Render;

private:
    Graphics _graphics;
};

}
