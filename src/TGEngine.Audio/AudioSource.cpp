#if TG_PLATFORM_MACOS
#include <OpenAL/alc.h>
#else
#include <AL/alc.h>
#endif

#include "AudioSource.h"
#include "AudioClip.h"
#include "OpenALDebug.h"

TG_NAMESPACE_BEGIN

AudioSource::AudioSource(ALuint alSource) noexcept :
    _alSource(alSource)
{
}

AudioSource::AudioSource(AudioSource&& rhs) noexcept :
    _alSource(rhs._alSource),
    _audioClip(std::move(rhs._audioClip))
{
    rhs._alSource = 0;
}

AudioSource::~AudioSource()
{
    if (_alSource != 0)
    {
        this->Stop();
        alDeleteSources(1, &_alSource);
    }
}

AudioSource& AudioSource::operator=(AudioSource&& rhs) noexcept
{
    std::swap(_alSource, rhs._alSource);
    std::swap(_audioClip, rhs._audioClip);

    return *this;
}

std::optional<AudioSource> AudioSource::Create()
{
    ALuint alSource;
    alGenSources(1, &alSource);
    if (alGetError() != AL_NO_ERROR)
    {
        return {};
    }

    return AudioSource(alSource);
}

void AudioSource::Play()
{
    this->SetProgressInSeconds(0.0f);
    TG_AL_ERROR_CHECK(alSourcePlay(_alSource))
}

bool AudioSource::IsPlaying() const
{
    ALint isPlaying = 0;
    TG_AL_ERROR_CHECK(alGetSourcei(_alSource, AL_SOURCE_STATE, &isPlaying))

    return isPlaying == AL_PLAYING;
}

void AudioSource::Stop()
{
    TG_AL_ERROR_CHECK(alSourceStop(_alSource))
}

void AudioSource::Pause()
{
    TG_AL_ERROR_CHECK(alSourcePause(_alSource))
}

void AudioSource::UnPause()
{
    TG_AL_ERROR_CHECK(alSourcePlay(_alSource))
}

void AudioSource::SetClip(const std::shared_ptr<AudioClip>& audioBuffer)
{
    TG_AL_ERROR_CHECK(alSourcei(_alSource, AL_BUFFER, audioBuffer->GetNativeBuffer()));
    _audioClip = audioBuffer;
}

void AudioSource::SetClip(std::shared_ptr<AudioClip>&& audioBuffer)
{
    TG_AL_ERROR_CHECK(alSourcei(_alSource, AL_BUFFER, audioBuffer->GetNativeBuffer()));
    _audioClip = std::move(audioBuffer);
}

void AudioSource::SetVolume(float volume)
{
    TG_AL_ERROR_CHECK(alSourcef(_alSource, AL_GAIN, volume))
}

void AudioSource::SetPosition(float x, float y, float z)
{
    TG_AL_ERROR_CHECK(alSource3f(_alSource, AL_POSITION, x, y, z))
}

void AudioSource::SetVelocity(float x, float y, float z)
{
    TG_AL_ERROR_CHECK(alSource3f(_alSource, AL_VELOCITY, x, y, z))
}

void AudioSource::SetListenerPosition(float x, float y, float z)
{
    TG_AL_ERROR_CHECK(alListener3f(AL_POSITION, x, y, z))
}

void AudioSource::SetListenerVelocity(float x, float y, float z)
{
    TG_AL_ERROR_CHECK(alListener3f(AL_VELOCITY, x, y, z))
}

float AudioSource::GetVolume() const
{
    float volume = 0.0f;
    TG_AL_ERROR_CHECK(alGetSourcef(_alSource, AL_GAIN, &volume))

    return volume;
}

void AudioSource::SetProgressInSeconds(float seconds)
{
    TG_AL_ERROR_CHECK(alSourcef(_alSource, AL_SEC_OFFSET, seconds))
}

float AudioSource::GetProgressInSeconds() const
{
    float seconds = 0;
    TG_AL_ERROR_CHECK(alGetSourcef(_alSource, AL_SEC_OFFSET, &seconds))

    return seconds;
}

float AudioSource::GetTotalProgressInSeconds() const
{
    if (_audioClip == nullptr)
    {
        return 0.0f;
    }
    
    return static_cast<float>(_audioClip->_audioDataBytes) / (_audioClip->GetSamplingRate() * _audioClip->GetChannels() * (static_cast<float>(_audioClip->GetBitsPerSample()) * 0.125f));
}

void AudioSource::SetPitch(float pitch)
{
    TG_AL_ERROR_CHECK(alSourcef(_alSource, AL_PITCH, pitch))
}

float AudioSource::GetPitch() const
{
    float pitch = 0.0f;
    TG_AL_ERROR_CHECK(alGetSourcef(_alSource, AL_PITCH, &pitch))

    return pitch;
}

std::shared_ptr<AudioClip> AudioSource::GetClip() noexcept
{
    return std::shared_ptr<AudioClip>();
}

std::shared_ptr<const AudioClip> AudioSource::GetClip() const noexcept
{
    return std::shared_ptr<const AudioClip>();
}

void AudioSource::SetLoop(bool isLoop)
{
    TG_AL_ERROR_CHECK(alSourcei(_alSource, AL_LOOPING, isLoop ? AL_TRUE : AL_FALSE))
}

bool AudioSource::IsLoop() const
{
    ALint isLoop = AL_FALSE;
    TG_AL_ERROR_CHECK(alGetSourcei(_alSource, AL_LOOPING, &isLoop))

    return isLoop == AL_TRUE ? true : false;
}

TG_NAMESPACE_END