#pragma once

#include <cstdint>

namespace tg
{

class AudioDecoder
{
/**@section Constructor */
public:
    AudioDecoder(std::shared_ptr<std::byte> audioData, int32_t audioDataBytes, int32_t bitsPerSample, int32_t channels, int32_t samplingRate) noexcept;
    
/**@section Method */
public:
    [[nodiscard]] std::shared_ptr<std::byte> GetAudioData() noexcept;
    [[nodiscard]] std::shared_ptr<const std::byte> GetAudioData() const noexcept;
    [[nodiscard]] int32_t GetAudioDataBytes() const noexcept;
    [[nodiscard]] int32_t GetBitsPerSample() const noexcept;
    [[nodiscard]] int32_t GetChannels() const noexcept;
    [[nodiscard]] int32_t GetSamplingRate() const noexcept;

/**@section Variable */
protected:
    std::shared_ptr<std::byte> m_audioData;
    int32_t m_audioDataBytes = 0;
    int32_t m_bitsPerSample = 0;
    int32_t m_channels = 0;
    int32_t m_samplingRate = 0;
};

inline AudioDecoder::AudioDecoder(std::shared_ptr<std::byte> audioData, int32_t audioDataBytes, int32_t bitsPerSample, int32_t channels, int32_t samplingRate) noexcept :
    m_audioData(std::move(audioData)),
    m_audioDataBytes(audioDataBytes),
    m_bitsPerSample(bitsPerSample),
    m_channels(channels),
    m_samplingRate(samplingRate)
{
}

inline std::shared_ptr<std::byte> AudioDecoder::GetAudioData() noexcept
{
    return m_audioData;
}

inline std::shared_ptr<const std::byte> AudioDecoder::GetAudioData() const noexcept
{
    return m_audioData;
}

inline int32_t AudioDecoder::GetAudioDataBytes() const noexcept
{
    return m_audioDataBytes;
}

inline int32_t AudioDecoder::GetBitsPerSample() const noexcept
{
    return m_bitsPerSample;
}

inline int32_t AudioDecoder::GetChannels() const noexcept
{
    return m_channels;
}

inline int32_t AudioDecoder::GetSamplingRate() const noexcept
{
    return m_samplingRate;
}

}
