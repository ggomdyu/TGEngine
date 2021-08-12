#pragma once

#include <span>
#include <optional>

#include "AudioDecoder.h"

namespace tg
{

class VorbisAudioDecoder final :
    public AudioDecoder
{
/**@section Constructor */
public:
    using AudioDecoder::AudioDecoder;

/**@section Method */
public:
    static std::optional<VorbisAudioDecoder> Create(const std::span<const std::byte>& fileData);
    static bool IsVorbis(const std::span<const std::byte>& fileData);
};

}
