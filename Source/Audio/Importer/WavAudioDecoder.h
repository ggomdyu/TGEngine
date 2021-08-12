#pragma once

#include <span>
#include <optional>

#include "AudioDecoder.h"

namespace tg
{

class WavAudioDecoder final :
    public AudioDecoder
{
/**@section Constructor */
public:
    using AudioDecoder::AudioDecoder;

/**@section Method */
public:
    static std::optional<WavAudioDecoder> Create(const std::span<const std::byte>& fileData);
    static bool IsWav(const std::span<const std::byte>& fileData);
};

}
