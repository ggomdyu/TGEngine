#include "PrecompiledHeader.h"

#include "AudioModule.h"

namespace tg
{

AudioModule::AudioModule() :
    m_device({nullptr, [](ALCdevice* device)
    {
        alcCloseDevice(device);
    }}),
    m_context({nullptr, [](ALCcontext* context)
    {
        alcMakeContextCurrent(nullptr);
        alcDestroyContext(context);
    }})
{
}

void AudioModule::Update()
{
}

}
