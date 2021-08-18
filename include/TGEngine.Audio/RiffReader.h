#pragma once

#include <cstdint>

TG_NAMESPACE_BEGIN

class RiffReader
{
public:
    enum ChunkId : uint32_t
    {
//#if TG_USING_LITTLE_ENDIAN
        Riff = 'FFIR',
        Wave = 'EVAW',
        Cue = ' euc',
        Fact = 'tcaf',
        Plst = 'tslp',
        Data = 'atad',
        Fmt = ' tmf',
        List = 'TSIL',
        Label = 'lbal',
        Ltxt = 'txtl',
        Note = 'eton',
        Smpl = 'lpms',
        Inst = 'tsni',
//#else
//#endif
    };

    struct ChunkHeader
    {
        public:
        ChunkHeader(ChunkId chunkId, uint32_t chunkDataSize, const std::byte* chunkData) noexcept;
        
        public:
        std::size_t GetSize() const noexcept;
        
        public:
        ChunkId chunkId;
        uint32_t chunkDataSize;
        const std::byte* chunkData;
    };

    struct RIFFChunkData
    {
        uint32_t riffType;
    };

    struct FmtChunkData
    {
        enum class AudioFormat : int16_t
        {
            PCM = 0x0001,
            IEEEFloat = 0x0003,
            ALaw = 0x0006,
            MuLaw = 0x0007,
        };

        enum class AudioChannel : int16_t
        {
            Mono = 1,
            Stereo = 2,
        };

        AudioFormat audioFormat;
        AudioChannel channels;
        int32_t samplingRate;
        int32_t avgBytesPerSec; // samplingRate * channels * bitsPerSample / 8
        int16_t blockAlign; // channels * bitsPerSample / 8
        int16_t bitsPerSample;
    };

    struct ListChunkData
    {
        uint32_t listType;
    };

    struct DataChunkData
    {
    };

public:
    RiffReader(const std::byte* fileData, std::size_t fileDataBytes) noexcept;
    
public:
    bool ReadNext();
    ChunkHeader GetChunkHeader() const;

private:
    const std::byte* _fileData;
    const std::byte* _fileDataIt;
    std::size_t _fileDataBytes;
};

inline RiffReader::ChunkHeader::ChunkHeader(ChunkId chunkId, uint32_t chunkDataSize, const std::byte* chunkData) noexcept :
    chunkId(chunkId),
    chunkDataSize(chunkDataSize),
    chunkData(chunkData)
{
}
    
inline std::size_t RiffReader::ChunkHeader::GetSize() const noexcept
{
    auto chunkSize = sizeof(chunkId) + sizeof(chunkDataSize) + sizeof(uint32_t);
    if (chunkId != ChunkId::Riff)
    {
        chunkSize -= sizeof(uint32_t) - chunkDataSize;
    }
    
    return chunkSize;
}

inline RiffReader::RiffReader(const std::byte* fileData, std::size_t fileDataBytes) noexcept :
    _fileData(fileData),
    _fileDataIt(fileData),
    _fileDataBytes(fileDataBytes)
{
}

inline bool RiffReader::ReadNext()
{
    ChunkHeader currChunk = GetChunkHeader();

    _fileDataIt += currChunk.GetSize();
    if (_fileDataIt >= _fileData + _fileDataBytes) // This not works fine. Should be fixed!
    {
        return false;
    }

    return true;
}

inline RiffReader::ChunkHeader RiffReader::GetChunkHeader() const
{
    return ChunkHeader(static_cast<ChunkId>(*reinterpret_cast<const uint32_t*>(&_fileDataIt[0])), *reinterpret_cast<const uint32_t*>(&_fileDataIt[4]), &_fileDataIt[8]);
}

}
