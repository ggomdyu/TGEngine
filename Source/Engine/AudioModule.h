#pragma once

#include "Engine/Module.h"

namespace tg
{

class AudioModule :
	public Module
{
public:
    TGON_RTTI(AudioModule)

/* @section Constructor */
public:
    AudioModule();

/* @section Method */
public:
    void Update() override;

/* @section Variable */
public:
    static constexpr auto ModuleStage = ModuleStage::Idle;

private:
    std::unique_ptr<struct ALCdevice, void(*)(ALCdevice*)> m_device;
    std::unique_ptr<struct ALCcontext, void(*)(ALCcontext*)> m_context;
};

}
