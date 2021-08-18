#include "AudioModule.h"

TG_NAMESPACE_BEGIN

AudioModule::AudioModule() :
    _device({nullptr, [](ALCdevice* device)
    {
        alcCloseDevice(device);
    }}),
    _context({nullptr, [](ALCcontext* context)
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
