#pragma once

#include <vector>
#include <string>

#include "Stream.h"

namespace tg
{

enum class FileMode
{
    CreateNew = 1,
    Create = 2,
    Open = 3,
    OpenOrCreate = 4,
    Truncate = 5,
    Append = 6,
};

enum class FileAccess
{
    Read = 1,
    Write = 2,
    ReadWrite = 3
};

enum class FileShare
{
    None = 0,
    Read = 1,
    Write = 2,
    ReadWrite = 3,
    Delete = 4,
    Inheritable = 16
};

enum class FileOptions
{
    WriteThrough = std::numeric_limits<int>::min(),
    None = 0,
    Encrypted = 16384,
    DeleteOnClose = 67108864,
    SequentialScan = 134217728,
    RandomAccess = 268435456,
    Asynchronous = 1073741824
};

class FileStream :
    public Stream
{
/**@section Constructor */
protected:
    FileStream(void* nativeFileHandle, const char8_t* path, FileAccess access, int32_t bufferSize);

public:
    FileStream(const FileStream& rhs) = delete;
    FileStream(FileStream&& rhs) noexcept;

/**@section Destructor */
public:
    ~FileStream() override;
    
/**@section Operator */
public:
    FileStream& operator=(const FileStream& rhs) = delete;
    FileStream& operator=(FileStream&& rhs) noexcept;
    bool operator==(const FileStream& rhs) const noexcept;
    bool operator!=(const FileStream& rhs) const noexcept;

/**@section Method */
public:
    [[nodiscard]] static std::optional<FileStream> Create(const char8_t* path, FileMode mode);
    [[nodiscard]] static std::optional<FileStream> Create(const char8_t* path, FileMode mode, FileAccess access);
    [[nodiscard]] static std::optional<FileStream> Create(const char8_t* path, FileMode mode, FileAccess access, FileShare share);
    [[nodiscard]] static std::optional<FileStream> Create(const char8_t* path, FileMode mode, FileAccess access, FileShare share, int32_t bufferSize);
    [[nodiscard]] static std::optional<FileStream> Create(const char8_t* path, FileMode mode, FileAccess access, FileShare share, int32_t bufferSize, FileOptions options);
    [[nodiscard]] bool CanRead() const override;
    [[nodiscard]] bool CanSeek() const override;
    [[nodiscard]] bool CanWrite() const override;
    bool SetLength(int64_t value) override;
    [[nodiscard]] int64_t Length() const override;
    [[nodiscard]] int64_t Position() const override;
    int32_t Read(std::byte* buffer, int32_t count) override;
    int32_t ReadByte() override;
    bool Write(const std::byte* buffer, int32_t count) override;
    bool WriteByte(std::byte value) override;
    int64_t Seek(int64_t offset, SeekOrigin origin) override;
    void Close() override;
    [[nodiscard]] const std::u8string& Name() const noexcept;
    void Flush() override;
    void Flush(bool flushToDisk);
    [[nodiscard]] bool IsClosed() const noexcept;
    
protected:
    [[nodiscard]] std::vector<std::byte>& GetBuffer();
    void FlushWriteBuffer();
    void FlushReadBuffer();
    int32_t InternalRead(std::byte* buffer, int32_t count);
    int32_t InternalWrite(const std::byte* buffer, int32_t count);
    int64_t InternalSeek(int64_t offset, SeekOrigin origin);
    void InternalFlush();
    bool InternalSetLength(int64_t value);

/**@section Variable */
protected:
    static constexpr FileShare DefaultShare = FileShare::Read;
    static constexpr FileOptions DefaultFileOption = FileOptions::None;
    static constexpr int DefaultBufferSize = 4096;

    void* m_nativeFileHandle;
    std::vector<std::byte> m_buffer;
    int32_t m_bufferSize;
    int32_t m_readPos;
    int32_t m_readLen;
    int32_t m_writePos;
    int64_t m_filePos;
    FileAccess m_access;
    std::u8string m_fileName;    
};

}
