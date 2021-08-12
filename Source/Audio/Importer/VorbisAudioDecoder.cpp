#include "PrecompiledHeader.h"

#include <stb_vorbis.c>

#include "VorbisAudioDecoder.h"

namespace tg
{

std::optional<VorbisAudioDecoder> VorbisAudioDecoder::Create(const std::span<const std::byte>& fileData)
{
    if (IsVorbis(fileData) == false)
    {
        return {};
    }

    short* audioData = nullptr;
    int channels = 0;
    int samplingRate = 0;
    const int audioDataBytes = stb_vorbis_decode_memory(reinterpret_cast<const unsigned char*>(fileData.data()), static_cast<int>(fileData.size()), &channels, &samplingRate, &audioData) * 4;
    
    const int32_t bitsPerSample = 16;
    return VorbisAudioDecoder(std::shared_ptr<std::byte>(reinterpret_cast<std::byte*>(audioData)), static_cast<int32_t>(audioDataBytes), bitsPerSample, static_cast<int32_t>(channels), static_cast<int32_t>(samplingRate));
}

bool VorbisAudioDecoder::IsVorbis(const std::span<const std::byte>& fileData)
{
    if (fileData.size() < 35)
    {
        return false;
    }
    
    return !strncmp(reinterpret_cast<const char*>(&fileData[0]), "OggS", 4);
}

}
