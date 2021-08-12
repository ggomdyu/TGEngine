#pragma once

#include <span>

namespace tg
{

enum class SeekOrigin
{
    Begin,
    Current,
    End
};
 
class Stream
{
/**@section Constructor */
public:
    Stream() noexcept = default;
    Stream(const Stream& rhs) = delete;
    Stream(Stream&& rhs) noexcept = default;

/**@section Destructor */
public:
    virtual ~Stream() = default;

/**@section Operator */
public:
    Stream& operator=(const Stream& rhs) = delete;
    Stream& operator=(Stream&& rhs) noexcept = default;

/**@section Method */
public:
    [[nodiscard]] virtual bool CanRead() const = 0;
    [[nodiscard]] virtual bool CanSeek() const = 0;
    [[nodiscard]] virtual bool CanWrite() const = 0;
    virtual bool SetLength(int64_t value) = 0;
    [[nodiscard]] virtual int64_t Length() const = 0;
    [[nodiscard]] virtual int64_t Position() const = 0;
    int32_t Read(const std::span<std::byte>& buffer);
    virtual int32_t Read(std::byte* buffer, int32_t count) = 0;
    virtual int32_t ReadByte() = 0;
    bool Write(const std::span<const std::byte>& buffer);
    virtual bool Write(const std::byte* buffer, int32_t count) = 0;
    virtual bool WriteByte(std::byte value) = 0;
    virtual int64_t Seek(int64_t offset, SeekOrigin origin) = 0;
    virtual void Close() = 0;
    virtual void Flush() = 0;
};
    
inline int32_t Stream::Read(const std::span<std::byte>& buffer)
{
    return this->Read(&buffer[0], static_cast<int32_t>(buffer.size()));
}

inline bool Stream::Write(const std::span<const std::byte>& buffer)
{
    return this->Write(&buffer[0], static_cast<int32_t>(buffer.size()));
}

}
