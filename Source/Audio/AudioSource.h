#pragma once

#include <optional>
#include <memory>

typedef unsigned int ALuint;

namespace tg
{

class AudioClip;

class AudioSource final
{
/**@section Constructor */
public:
    AudioSource(ALuint alSource) noexcept;
    AudioSource(const AudioSource& rhs) = delete;
    AudioSource(AudioSource&& rhs) noexcept;

/**@section Destructor */
public:
    ~AudioSource();

/**@section Operator */
public:
    AudioSource& operator=(const AudioSource& rhs) = delete;
    AudioSource& operator=(AudioSource&& rhs) noexcept;

/**@section Method */
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
    [[nodiscard]] float GetVolume() const;
    [[nodiscard]] float GetProgressInSeconds() const;
    [[nodiscard]] float GetTotalProgressInSeconds() const;
    [[nodiscard]] float GetPitch() const;
    [[nodiscard]] std::shared_ptr<AudioClip> GetClip() noexcept;
    [[nodiscard]] std::shared_ptr<const AudioClip> GetClip() const noexcept;
    [[nodiscard]] bool IsLoop() const;
    [[nodiscard]] bool IsPlaying() const;

/**@section Variable */
private:
    ALuint m_alSource;
    std::shared_ptr<AudioClip> m_audioClip;
};

}
