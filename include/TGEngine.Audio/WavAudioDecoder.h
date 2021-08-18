#pragma once

#include <span>
#include <optional>

#include "AudioDecoder.h"

TG_NAMESPACE_BEGIN

class WavAudioDecoder final :
    public AudioDecoder
{
public:
    using AudioDecoder::AudioDecoder;

public:
    static std::optional<WavAudioDecoder> Create(const std::span<const std::byte>& fileData);
    static bool IsWav(const std::span<const std::byte>& fileData);
};

}
