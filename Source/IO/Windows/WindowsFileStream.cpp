#include "PrecompiledHeader.h"

#include "Core/Algorithm.h"
#include "Platform/Windows/Windows.h"
#include "Text/Encoding.h"

#include "../FileStream.h"

namespace tg
{

extern thread_local std::array<wchar_t, 32768> g_tempUtf16StrBuffer;

namespace
{

HANDLE CreateFileOpenHandle(const char8_t* path, FileMode mode, FileAccess access, FileShare share, FileOptions options)
{
    if (Encoding::Convert(Encoding::UTF8(), Encoding::Unicode(), reinterpret_cast<const std::byte*>(path), static_cast<int32_t>(std::char_traits<char8_t>::length(path)), reinterpret_cast<std::byte*>(g_tempUtf16StrBuffer.data()), static_cast<int32_t>(g_tempUtf16StrBuffer.size())).has_value() == false)
    {
        return INVALID_HANDLE_VALUE;
    }

    const auto desiredAccess = (access == FileAccess::Read) ? GENERIC_READ : (access == FileAccess::Write) ? GENERIC_WRITE : GENERIC_READ | GENERIC_WRITE;

    const bool useSecurityAttributes = UnderlyingCast(share) & UnderlyingCast(FileShare::Inheritable);
    SECURITY_ATTRIBUTES securityAttributes{};
    if (useSecurityAttributes)
    {
        if ((UnderlyingCast(share) & UnderlyingCast(FileShare::Inheritable)) != 0)
        {
            securityAttributes.nLength = sizeof(SECURITY_ATTRIBUTES);
            securityAttributes.bInheritHandle = TRUE;
        }
    }

    // The named pipe on Windows allows the server to impersonate the client.
    // So we have to add the below flags because of this security vulnerability.
    const DWORD flagsAndAttributes = UnderlyingCast(options) | SECURITY_SQOS_PRESENT | SECURITY_ANONYMOUS;

    return CreateFileW(reinterpret_cast<LPCWSTR>(g_tempUtf16StrBuffer.data()), desiredAccess, static_cast<DWORD>(share), useSecurityAttributes ? &securityAttributes : nullptr, static_cast<DWORD>(mode), flagsAndAttributes, nullptr);
}

}

std::optional<FileStream> FileStream::Create(const char8_t* path, FileMode mode, FileAccess access, FileShare share, int32_t bufferSize, FileOptions options)
{
    auto* nativeFileHandle = CreateFileOpenHandle(path, mode, access, share, options);
    if (nativeFileHandle == INVALID_HANDLE_VALUE)
    {
        return {};
    }

    if (GetFileType(nativeFileHandle) != FILE_TYPE_DISK)
    {
        return {};
    }

    return FileStream(nativeFileHandle, path, access, bufferSize);
}

void FileStream::Close()
{
    this->FlushWriteBuffer();

    if (m_nativeFileHandle != INVALID_HANDLE_VALUE)
    {
        CloseHandle(m_nativeFileHandle);
        m_nativeFileHandle = INVALID_HANDLE_VALUE;
    }
}

bool FileStream::IsClosed() const noexcept
{
    return m_nativeFileHandle == INVALID_HANDLE_VALUE;
}

int64_t FileStream::Length() const
{
    LARGE_INTEGER li;
    if (GetFileSizeEx(m_nativeFileHandle, &li) == FALSE)
    {
        return -1;
    }

    int64_t length = li.QuadPart;
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

    m_writePos = 0;
}

int32_t FileStream::InternalRead(std::byte* buffer, int32_t count)
{
    DWORD readBytes = 0;
    if (ReadFile(m_nativeFileHandle, buffer, count, &readBytes, nullptr) == FALSE)
    {
        return 0;
    }

    m_filePos += readBytes;
    return readBytes;
}

int32_t FileStream::InternalWrite(const std::byte* buffer, int32_t bufferBytes)
{
    DWORD writtenBytes = 0;
    if (WriteFile(m_nativeFileHandle, buffer, bufferBytes, &writtenBytes, nullptr) == FALSE)
    {
        return 0;
    }

    m_filePos += writtenBytes;
    return writtenBytes;
}

int64_t FileStream::InternalSeek(int64_t offset, SeekOrigin origin)
{
    LARGE_INTEGER distanceToMove;
    distanceToMove.QuadPart = offset;

    LARGE_INTEGER newFilePointer;
    if (SetFilePointerEx(m_nativeFileHandle, distanceToMove, &newFilePointer, static_cast<DWORD>(origin)) == FALSE)
    {
        return 0;
    }

    m_filePos = newFilePointer.QuadPart;

    return newFilePointer.QuadPart;
}

bool FileStream::InternalSetLength(int64_t value)
{
    const auto prevFilePos = m_filePos;
    if (m_filePos != value)
    {
        if (this->InternalSeek(value, SeekOrigin::Begin) == -1)
        {
            return false;
        }
    }

    if (SetEndOfFile(m_nativeFileHandle) == FALSE)
    {
        return false;
    }

    if (prevFilePos != value)
    {
        if (value <= prevFilePos)
        {
            if (this->InternalSeek(0, SeekOrigin::End) == false)
            {
                return false;
            }
        }
        else
        {
            // Roll back the seek position
            if (this->InternalSeek(prevFilePos, SeekOrigin::Begin))
            {
                return false;
            }
        }
    }

    return true;
}

void FileStream::InternalFlush()
{
    FlushFileBuffers(m_nativeFileHandle);
}

}
