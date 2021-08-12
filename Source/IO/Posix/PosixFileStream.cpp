#include "PrecompiledHeader.h"

#include <cstdio>

#include "Diagnostics/Debug.h"

#include "../File.h"
#include "../FileStream.h"

namespace tg
{
namespace
{

constexpr const char* ConvertFileModeAccessToNative(FileMode mode, FileAccess access)
{
    constexpr const char* fileModeTable[][3] = {
        {nullptr, "wb", "wb+"},
        {nullptr, "wb", "wb+"},
        {"rb", "wb", "rb+"},
        {"rb", "wb", "rb+"},
        {nullptr, "wb", "wb+"},
        {nullptr, "ab", "ab+"},
    };

    return fileModeTable[static_cast<int32_t>(mode) - 1][static_cast<int32_t>(access) - 1];
}

FILE* CreateFileOpenHandle(const char8_t* path, FileMode mode, FileAccess access)
{
    if (mode == FileMode::OpenOrCreate && File::Exists(path) == false)
    {
        FileStream::Create(path, FileMode::CreateNew);
    }

    return fopen(reinterpret_cast<const char*>(path), ConvertFileModeAccessToNative(mode, access));
}

}

std::optional<FileStream> FileStream::Create(const char8_t* path, FileMode mode, FileAccess access, FileShare share, int32_t bufferSize, FileOptions options)
{
    auto* nativeFileHandle = CreateFileOpenHandle(path, mode, access);
    if (nativeFileHandle == nullptr)
    {
        return {};
    }
    
    return FileStream(nativeFileHandle, path, access, bufferSize);
}

void FileStream::Close()
{
    this->FlushWriteBuffer();

    if (m_nativeFileHandle != nullptr)
    {
        fclose(reinterpret_cast<FILE*>(m_nativeFileHandle));
        m_nativeFileHandle = nullptr;
    }
}

bool FileStream::IsClosed() const noexcept
{
    return m_nativeFileHandle == nullptr;
}

int64_t FileStream::Length() const
{
    FILE* nativeFileHandle = reinterpret_cast<FILE*>(m_nativeFileHandle);

    const auto prevSeekOffset = ftell(nativeFileHandle);
    fseek(nativeFileHandle, 0, SEEK_END);

    auto length = ftell(nativeFileHandle);
    fseek(nativeFileHandle, prevSeekOffset, SEEK_SET);

    if (m_filePos + m_writePos > length)
    {
        length = m_filePos + m_writePos;
    }

    return length;
}

void FileStream::FlushWriteBuffer()
{
    if (m_writePos <= 0)
    {
        return;
    }

    this->InternalWrite(&m_buffer[0], m_writePos);

    fflush(reinterpret_cast<FILE*>(m_nativeFileHandle));

    m_writePos = 0;
}

int32_t FileStream::InternalRead(std::byte* buffer, int32_t count)
{
    auto readBytes = fread(buffer, 1, count, reinterpret_cast<FILE*>(m_nativeFileHandle));
    if (readBytes == static_cast<decltype(readBytes)>(-1))
    {
        return 0;
    }

    m_filePos += readBytes;
    return static_cast<int32_t>(readBytes);
}

int32_t FileStream::InternalWrite(const std::byte* buffer, int32_t count)
{
    auto writtenBytes = fwrite(buffer, 1, count, reinterpret_cast<FILE*>(m_nativeFileHandle));
    if (writtenBytes == static_cast<decltype(writtenBytes)>(-1))
    {
        return 0;
    }

    m_filePos += writtenBytes;
    return static_cast<int32_t>(writtenBytes);
}

int64_t FileStream::InternalSeek(int64_t offset, SeekOrigin origin)
{
    auto nativeFileHandle = reinterpret_cast<FILE*>(m_nativeFileHandle);
    auto result = fseek(nativeFileHandle, offset, static_cast<int>(origin));
    if (result != 0)
    {
        return 0;
    }

    int64_t newFilePos = ftell(nativeFileHandle);
    if (newFilePos == -1)
    {
        return 0;
    }

    m_filePos = newFilePos;

    return static_cast<int32_t>(newFilePos);
}

bool FileStream::InternalSetLength(int64_t value)
{
    return false;
}

void FileStream::InternalFlush()
{
    fflush(reinterpret_cast<FILE*>(m_nativeFileHandle));
}

}
