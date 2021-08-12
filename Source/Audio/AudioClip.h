#pragma once

#include <span>
#include <memory>

typedef unsigned int ALuint;

namespace tg
{

enum class AudioFormat
{
    Unknown,
    Wav,
    Vorbis,
    Mp3,
    Flac,
    M4a,
    Opus,
};

class AudioClip final
{
    friend class AudioSource;

/**@section Constructor */
public:
    AudioClip(std::shared_ptr<std::byte> audioData, int32_t audioDataBytes, int32_t bitsPerSample, int32_t channels, int32_t samplingRate) noexcept;
    AudioClip(const AudioClip& rhs) = delete;
    AudioClip(AudioClip&& rhs) noexcept;
    
/**@section Operator */
public:
    AudioClip& operator=(const AudioClip& rhs) noexcept = delete;
    AudioClip& operator=(AudioClip&& rhs) noexcept;

/**@section Destructor */
public:
    ~AudioClip();

/**@section Method */
public:
    static std::shared_ptr<AudioClip> Create(const char8_t* path);
    static std::shared_ptr<AudioClip> Create(const std::span<const std::byte>& audioData);
    static std::shared_ptr<AudioClip> Create(const std::span<const std::byte>& audioData, AudioFormat audioFormat);
    [[nodiscard]] int32_t GetBitsPerSample() const noexcept;
    [[nodiscard]] int32_t GetChannels() const noexcept;
    [[nodiscard]] int32_t GetSamplingRate() const noexcept;
    [[nodiscard]] ALuint GetNativeBuffer() const noexcept;
   
/**@section Variable */
protected:
    ALuint m_alBufferId = 0;
    std::shared_ptr<std::byte> m_audioData;
    int32_t m_audioDataBytes = 0;
    int32_t m_bitsPerSample = 0;
    int32_t m_channels = 0;
    int32_t m_samplingRate = 0;
};

}
