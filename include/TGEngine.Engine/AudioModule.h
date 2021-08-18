#pragma once

#include "Engine/Module.h"

TG_NAMESPACE_BEGIN

class AudioModule :
	public Module
{
public:
    TG_OBJECT(AudioModule)

/* @section Constructor */
public:
    AudioModule();

public:
    void Update() override;

public:
    static constexpr auto ModuleStage = ModuleStage::Idle;

private:
    std::unique_ptr<struct ALCdevice, void(*)(ALCdevice*)> _device;
    std::unique_ptr<struct ALCcontext, void(*)(ALCcontext*)> _context;
};

}
