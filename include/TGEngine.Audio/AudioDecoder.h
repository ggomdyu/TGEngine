#pragma once

#include <cstdint>

TG_NAMESPACE_BEGIN

class AudioDecoder
{
public:
    AudioDecoder(std::shared_ptr<std::byte> audioData, int32_t audioDataBytes, int32_t bitsPerSample, int32_t channels, int32_t samplingRate) noexcept;
    
public:
    std::shared_ptr<std::byte> GetAudioData() noexcept;
    std::shared_ptr<const std::byte> GetAudioData() const noexcept;
    int32_t GetAudioDataBytes() const noexcept;
    int32_t GetBitsPerSample() const noexcept;
    int32_t GetChannels() const noexcept;
    int32_t GetSamplingRate() const noexcept;

protected:
    std::shared_ptr<std::byte> _audioData;
    int32_t _audioDataBytes = 0;
    int32_t _bitsPerSample = 0;
    int32_t _channels = 0;
    int32_t _samplingRate = 0;
};

inline AudioDecoder::AudioDecoder(std::shared_ptr<std::byte> audioData, int32_t audioDataBytes, int32_t bitsPerSample, int32_t channels, int32_t samplingRate) noexcept :
    _audioData(std::move(audioData)),
    _audioDataBytes(audioDataBytes),
    _bitsPerSample(bitsPerSample),
    _channels(channels),
    _samplingRate(samplingRate)
{
}

inline std::shared_ptr<std::byte> AudioDecoder::GetAudioData() noexcept
{
    return _audioData;
}

inline std::shared_ptr<const std::byte> AudioDecoder::GetAudioData() const noexcept
{
    return _audioData;
}

inline int32_t AudioDecoder::GetAudioDataBytes() const noexcept
{
    return _audioDataBytes;
}

inline int32_t AudioDecoder::GetBitsPerSample() const noexcept
{
    return _bitsPerSample;
}

inline int32_t AudioDecoder::GetChannels() const noexcept
{
    return _channels;
}

inline int32_t AudioDecoder::GetSamplingRate() const noexcept
{
    return _samplingRate;
}

}
