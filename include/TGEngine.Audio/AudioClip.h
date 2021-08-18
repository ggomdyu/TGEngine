#pragma once

#include <span>
#include <memory>

typedef unsigned int ALuint;

TG_NAMESPACE_BEGIN

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

public:
    AudioClip(std::shared_ptr<std::byte> audioData, int32_t audioDataBytes, int32_t bitsPerSample, int32_t channels, int32_t samplingRate) noexcept;
    AudioClip(const AudioClip& rhs) = delete;
    AudioClip(AudioClip&& rhs) noexcept;
    ~AudioClip();
    
public:
    AudioClip& operator=(const AudioClip& rhs) noexcept = delete;
    AudioClip& operator=(AudioClip&& rhs) noexcept;

public:
    static std::shared_ptr<AudioClip> Create(const char8_t* path);
    static std::shared_ptr<AudioClip> Create(const std::span<const std::byte>& audioData);
    static std::shared_ptr<AudioClip> Create(const std::span<const std::byte>& audioData, AudioFormat audioFormat);
    int32_t GetBitsPerSample() const noexcept;
    int32_t GetChannels() const noexcept;
    int32_t GetSamplingRate() const noexcept;
    ALuint GetNativeBuffer() const noexcept;
   
protected:
    ALuint _alBufferId = 0;
    std::shared_ptr<std::byte> _audioData;
    int32_t _audioDataBytes = 0;
    int32_t _bitsPerSample = 0;
    int32_t _channels = 0;
    int32_t _samplingRate = 0;
};

}
