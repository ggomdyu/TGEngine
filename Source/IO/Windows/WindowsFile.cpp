#include "PrecompiledHeader.h"

#include <array>

#include "Platform/Windows/SafeHandle.h"
#include "Platform/Windows/Windows.h"

#include "../File.h"

namespace tg
{

extern thread_local std::array<wchar_t, 32768> g_tempUtf16StrBuffer;

namespace
{

#if !defined(S_ISREG)
constexpr bool S_ISREG(unsigned short m) noexcept
{
    return (m & S_IFMT) == S_IFREG;
}
#endif

std::optional<struct _stat> CreateStat(const char8_t* path, const std::span<wchar_t>& utf16PathBuffer)
{
    if (MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(path), -1, &utf16PathBuffer[0], static_cast<int>(utf16PathBuffer.size())) == 0)
    {
        return {};
    }

    struct _stat s{};
    if (_wstat(&utf16PathBuffer[0], &s) != 0)
    {
        return {};
    }

    return s;
}

}

bool File::Copy(const char8_t* srcPath, const char8_t* destPath, bool overwrite)
{
    const auto utf16SrcPath = std::span(g_tempUtf16StrBuffer).subspan(0, g_tempUtf16StrBuffer.size() / 2);
    if (MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(srcPath), -1, utf16SrcPath.data(), static_cast<int>(utf16SrcPath.size())) == 0)
    {
        return false;
    }

    const auto utf16DestPath = std::span(g_tempUtf16StrBuffer).subspan(g_tempUtf16StrBuffer.size() / 2, g_tempUtf16StrBuffer.size() / 2);
    if (MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(destPath), -1, utf16DestPath.data(), static_cast<int>(utf16DestPath.size())) == 0)
    {
        return false;
    }

    return CopyFileW(utf16SrcPath.data(), utf16DestPath.data(), overwrite ? FALSE : TRUE) != 0;
}

bool File::Delete(const char8_t* path)
{
    auto s = CreateStat(path, g_tempUtf16StrBuffer);
    if (s.has_value() == false || S_ISREG(s->st_mode) == false)
    {
        return false;
    }

    return _wremove(g_tempUtf16StrBuffer.data()) == 0;
}

bool File::Exists(const char8_t* path)
{
    auto s = CreateStat(path, g_tempUtf16StrBuffer);
    if (s.has_value() == false || S_ISREG(s->st_mode) == false)
    {
        return false;
    }

    return true;
}

bool File::Move(const char8_t* srcPath, const char8_t* destPath)
{
    const auto utf16SrcPath = std::span(g_tempUtf16StrBuffer).subspan(0, g_tempUtf16StrBuffer.size() / 2);
    auto s = CreateStat(srcPath, utf16SrcPath);
    if (s.has_value() == false || S_ISREG(s->st_mode) == false)
    {
        return false;
    }

    const auto utf16DestPath = std::span(g_tempUtf16StrBuffer).subspan(g_tempUtf16StrBuffer.size() / 2, g_tempUtf16StrBuffer.size() / 2);
    if (MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(destPath), -1, utf16DestPath.data(), static_cast<int>(utf16DestPath.size())) == 0)
    {
        return {};
    }

    return _wrename(utf16SrcPath.data(), utf16DestPath.data()) == 0;
}

std::optional<DateTime> File::GetLastAccessTimeUtc(const char8_t* path)
{
    auto s = CreateStat(path, g_tempUtf16StrBuffer);
    if (s.has_value() == false)
    {
        return {};
    }

    return DateTime(DateTime::GetUnixEpoch().GetTicks() + TimeSpan::TicksPerSecond * s->st_atime);
}

std::optional<DateTime> File::GetLastWriteTimeUtc(const char8_t* path)
{
    auto s = CreateStat(path, g_tempUtf16StrBuffer);
    if (s.has_value() == false)
    {
        return {};
    }

    return DateTime(DateTime::GetUnixEpoch().GetTicks() + TimeSpan::TicksPerSecond * s->st_mtime);
}

bool File::SetCreationTimeUtc(const char8_t* path, const DateTime& creationTimeUtc)
{
    if (MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(path), -1, g_tempUtf16StrBuffer.data(), static_cast<int>(g_tempUtf16StrBuffer.size())) == 0)
    {
        return {};
    }

    const SafeHandle handle(CreateFile2(g_tempUtf16StrBuffer.data(), GENERIC_WRITE, FILE_SHARE_READ, OPEN_EXISTING, nullptr));
    if (handle == nullptr)
    {
        return false;
    }

    const int64_t ticks = creationTimeUtc.ToFileTimeUtc();
    FILETIME fileTime = {
        .dwLowDateTime = static_cast<DWORD>(ticks),
        .dwHighDateTime = static_cast<DWORD>(ticks >> 32)
    };
    if (SetFileTime(handle, &fileTime, nullptr, nullptr) == 0)
    {
        return false;
    }

    return true;
}

bool File::SetLastAccessTimeUtc(const char8_t* path, const DateTime& lastAccessTimeUtc)
{
    if (MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(path), -1, g_tempUtf16StrBuffer.data(), static_cast<int>(g_tempUtf16StrBuffer.size())) == 0)
    {
        return {};
    }

    const SafeHandle handle(CreateFile2(g_tempUtf16StrBuffer.data(), GENERIC_WRITE, FILE_SHARE_READ, OPEN_EXISTING, nullptr));
    if (handle == nullptr)
    {
        return false;
    }

    const auto ticks = lastAccessTimeUtc.ToFileTimeUtc();
    FILETIME fileTime{static_cast<DWORD>(ticks), static_cast<DWORD>(ticks >> 32)};
    if (SetFileTime(handle, nullptr, &fileTime, nullptr) == 0)
    {
        return false;
    }

    return true;
}

bool File::SetLastWriteTimeUtc(const char8_t* path, const DateTime& lastWriteTimeUtc)
{
    if (MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(path), -1, g_tempUtf16StrBuffer.data(), static_cast<int>(g_tempUtf16StrBuffer.size())) == 0)
    {
        return {};
    }

    const SafeHandle handle(CreateFile2(g_tempUtf16StrBuffer.data(), GENERIC_WRITE, FILE_SHARE_READ, OPEN_EXISTING, nullptr));
    if (handle == nullptr)
    {
        return false;
    }

    const auto ticks = lastWriteTimeUtc.ToFileTimeUtc();
    const FILETIME fileTime{static_cast<DWORD>(ticks), static_cast<DWORD>(ticks >> 32)};
    if (SetFileTime(handle, nullptr, nullptr, &fileTime) == 0)
    {
        return false;
    }

    return true;
}

std::optional<DateTime> File::GetCreationTimeUtc(const char8_t* path)
{
    auto s = CreateStat(path, g_tempUtf16StrBuffer);
    if (s.has_value() == false)
    {
        return {};
    }

    return DateTime(DateTime::GetUnixEpoch().GetTicks() + TimeSpan::TicksPerSecond * s->st_ctime);
}

std::optional<FileAttributes> File::GetAttributes(const char8_t* path)
{
    if (MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(path), -1, g_tempUtf16StrBuffer.data(), static_cast<int>(g_tempUtf16StrBuffer.size())) == 0)
    {
        return {};
    }

    WIN32_FILE_ATTRIBUTE_DATA fileAttributeData;
    if (GetFileAttributesExW(g_tempUtf16StrBuffer.data(), GetFileExInfoStandard, &fileAttributeData) == FALSE)
    {
        return {};
    }

    return FileAttributes(fileAttributeData.dwFileAttributes);
}

bool File::Decrypt(const char8_t* path)
{
    if (MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(path), -1, g_tempUtf16StrBuffer.data(), static_cast<int>(g_tempUtf16StrBuffer.size())) == 0)
    {
        return {};
    }

    return !!DecryptFileW(g_tempUtf16StrBuffer.data(), 0);
}

bool File::Encrypt(const char8_t* path)
{
    if (MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(path), -1, g_tempUtf16StrBuffer.data(), static_cast<int>(g_tempUtf16StrBuffer.size())) == 0)
    {
        return {};
    }

    return !!EncryptFileW(g_tempUtf16StrBuffer.data());
}

bool File::SetAttributes(const char8_t* path, FileAttributes fileAttributes)
{
    if (MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(path), -1, g_tempUtf16StrBuffer.data(), static_cast<int>(g_tempUtf16StrBuffer.size())) == 0)
    {
        return {};
    }

    return !!SetFileAttributesW(g_tempUtf16StrBuffer.data(), static_cast<DWORD>(fileAttributes));
}

}
