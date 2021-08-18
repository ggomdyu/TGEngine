#pragma once

#include <span>
#include <optional>

#include "AudioDecoder.h"

TG_NAMESPACE_BEGIN

class VorbisAudioDecoder final :
    public AudioDecoder
{
public:
    using AudioDecoder::AudioDecoder;

public:
    static std::optional<VorbisAudioDecoder> Create(const std::span<const std::byte>& fileData);
    static bool IsVorbis(const std::span<const std::byte>& fileData);
};

}
