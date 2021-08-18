#pragma once

#include <optional>
#include <memory>

typedef unsigned int ALuint;

TG_NAMESPACE_BEGIN

class AudioClip;

class AudioSource final
{
public:
    AudioSource(ALuint alSource) noexcept;
    AudioSource(const AudioSource& rhs) = delete;
    AudioSource(AudioSource&& rhs) noexcept;
    ~AudioSource();

public:
    AudioSource& operator=(const AudioSource& rhs) = delete;
    AudioSource& operator=(AudioSource&& rhs) noexcept;

public:
    static std::optional<AudioSource> Create();
    void Play();
    void Stop();
    void Pause();
    void UnPause();
    void SetClip(const std::shared_ptr<AudioClip>& audioBuffer);
    void SetClip(std::shared_ptr<AudioClip>&& audioBuffer);
    void SetVolume(float volume);
    void SetPosition(float x, float y, float z);
    void SetVelocity(float x, float y, float z);
    static void SetListenerPosition(float x, float y, float z);
    static void SetListenerVelocity(float x, float y, float z);
    void SetProgressInSeconds(float seconds);
    void SetPitch(float pitch);
    void SetLoop(bool isLoop);
    float GetVolume() const;
    float GetProgressInSeconds() const;
    float GetTotalProgressInSeconds() const;
    float GetPitch() const;
    std::shared_ptr<AudioClip> GetClip() noexcept;
    std::shared_ptr<const AudioClip> GetClip() const noexcept;
    bool IsLoop() const;
    bool IsPlaying() const;

private:
    ALuint _alSource;
    std::shared_ptr<AudioClip> _audioClip;
};

}
