#include "PrecompiledHeader.h"

#if TGON_PLATFORM_MACOS
#include <OpenAL/alc.h>
#else
#include <AL/alc.h>
#endif

#include "AudioSource.h"
#include "AudioClip.h"
#include "OpenALDebug.h"

namespace tg
{

AudioSource::AudioSource(ALuint alSource) noexcept :
    m_alSource(alSource)
{
}

AudioSource::AudioSource(AudioSource&& rhs) noexcept :
    m_alSource(rhs.m_alSource),
    m_audioClip(std::move(rhs.m_audioClip))
{
    rhs.m_alSource = 0;
}

AudioSource::~AudioSource()
{
    if (m_alSource != 0)
    {
        this->Stop();
        alDeleteSources(1, &m_alSource);
    }
}

AudioSource& AudioSource::operator=(AudioSource&& rhs) noexcept
{
    std::swap(m_alSource, rhs.m_alSource);
    std::swap(m_audioClip, rhs.m_audioClip);

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
    TGON_AL_ERROR_CHECK(alSourcePlay(m_alSource))
}

bool AudioSource::IsPlaying() const
{
    ALint isPlaying = 0;
    TGON_AL_ERROR_CHECK(alGetSourcei(m_alSource, AL_SOURCE_STATE, &isPlaying))

    return isPlaying == AL_PLAYING;
}

void AudioSource::Stop()
{
    TGON_AL_ERROR_CHECK(alSourceStop(m_alSource))
}

void AudioSource::Pause()
{
    TGON_AL_ERROR_CHECK(alSourcePause(m_alSource))
}

void AudioSource::UnPause()
{
    TGON_AL_ERROR_CHECK(alSourcePlay(m_alSource))
}

void AudioSource::SetClip(const std::shared_ptr<AudioClip>& audioBuffer)
{
    TGON_AL_ERROR_CHECK(alSourcei(m_alSource, AL_BUFFER, audioBuffer->GetNativeBuffer()));
    m_audioClip = audioBuffer;
}

void AudioSource::SetClip(std::shared_ptr<AudioClip>&& audioBuffer)
{
    TGON_AL_ERROR_CHECK(alSourcei(m_alSource, AL_BUFFER, audioBuffer->GetNativeBuffer()));
    m_audioClip = std::move(audioBuffer);
}

void AudioSource::SetVolume(float volume)
{
    TGON_AL_ERROR_CHECK(alSourcef(m_alSource, AL_GAIN, volume))
}

void AudioSource::SetPosition(float x, float y, float z)
{
    TGON_AL_ERROR_CHECK(alSource3f(m_alSource, AL_POSITION, x, y, z))
}

void AudioSource::SetVelocity(float x, float y, float z)
{
    TGON_AL_ERROR_CHECK(alSource3f(m_alSource, AL_VELOCITY, x, y, z))
}

void AudioSource::SetListenerPosition(float x, float y, float z)
{
    TGON_AL_ERROR_CHECK(alListener3f(AL_POSITION, x, y, z))
}

void AudioSource::SetListenerVelocity(float x, float y, float z)
{
    TGON_AL_ERROR_CHECK(alListener3f(AL_VELOCITY, x, y, z))
}

float AudioSource::GetVolume() const
{
    float volume = 0.0f;
    TGON_AL_ERROR_CHECK(alGetSourcef(m_alSource, AL_GAIN, &volume))

    return volume;
}

void AudioSource::SetProgressInSeconds(float seconds)
{
    TGON_AL_ERROR_CHECK(alSourcef(m_alSource, AL_SEC_OFFSET, seconds))
}

float AudioSource::GetProgressInSeconds() const
{
    float seconds = 0;
    TGON_AL_ERROR_CHECK(alGetSourcef(m_alSource, AL_SEC_OFFSET, &seconds))

    return seconds;
}

float AudioSource::GetTotalProgressInSeconds() const
{
    if (m_audioClip == nullptr)
    {
        return 0.0f;
    }
    
    return static_cast<float>(m_audioClip->m_audioDataBytes) / (m_audioClip->GetSamplingRate() * m_audioClip->GetChannels() * (static_cast<float>(m_audioClip->GetBitsPerSample()) * 0.125f));
}

void AudioSource::SetPitch(float pitch)
{
    TGON_AL_ERROR_CHECK(alSourcef(m_alSource, AL_PITCH, pitch))
}

float AudioSource::GetPitch() const
{
    float pitch = 0.0f;
    TGON_AL_ERROR_CHECK(alGetSourcef(m_alSource, AL_PITCH, &pitch))

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
    TGON_AL_ERROR_CHECK(alSourcei(m_alSource, AL_LOOPING, isLoop ? AL_TRUE : AL_FALSE))
}

bool AudioSource::IsLoop() const
{
    ALint isLoop = AL_FALSE;
    TGON_AL_ERROR_CHECK(alGetSourcei(m_alSource, AL_LOOPING, &isLoop))

    return isLoop == AL_TRUE ? true : false;
}

}
