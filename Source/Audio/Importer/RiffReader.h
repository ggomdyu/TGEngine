#pragma once

#include <cstdint>

namespace tg
{

class RiffReader
{
/**@section Struct */
public:
    enum ChunkId : uint32_t
    {
//#if TGON_USING_LITTLE_ENDIAN
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
    /**@section Constructor */
    public:
        ChunkHeader(ChunkId chunkId, uint32_t chunkDataSize, const std::byte* chunkData) noexcept;
        
    /**@section Method */
    public:
        std::size_t GetSize() const noexcept;
        
    /**@section Variable */
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

/**@section Type */
public:
    RiffReader(const std::byte* fileData, std::size_t fileDataBytes) noexcept;
    
/**@section Method */
public:
    bool ReadNext();
    ChunkHeader GetChunkHeader() const;

/**@section Variable */
private:
    const std::byte* m_fileData;
    const std::byte* m_fileDataIt;
    std::size_t m_fileDataBytes;
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
    m_fileData(fileData),
    m_fileDataIt(fileData),
    m_fileDataBytes(fileDataBytes)
{
}

inline bool RiffReader::ReadNext()
{
    ChunkHeader currChunk = GetChunkHeader();

    m_fileDataIt += currChunk.GetSize();
    if (m_fileDataIt >= m_fileData + m_fileDataBytes) // This not works fine. Should be fixed!
    {
        return false;
    }

    return true;
}

inline RiffReader::ChunkHeader RiffReader::GetChunkHeader() const
{
    return ChunkHeader(static_cast<ChunkId>(*reinterpret_cast<const uint32_t*>(&m_fileDataIt[0])), *reinterpret_cast<const uint32_t*>(&m_fileDataIt[4]), &m_fileDataIt[8]);
}

}
