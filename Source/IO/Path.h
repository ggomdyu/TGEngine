#pragma once

#include <cstdint>
#include <string>
#include <span>

#if TGON_PLATFORM_WINDOWS
#include "Windows/WindowsPath.h"
#elif TGON_SUPPORT_POSIX
#include "Posix/PosixPath.h"
#endif

namespace tg
{

class Path :
    private PlatformPath
{
/**@section Constructor */
public:
    Path() = delete;
    
/**@section Method */
public:
    [[nodiscard]] static std::u8string Combine(const std::u8string_view& path1, const std::u8string_view& path2);
    static std::optional<int32_t> Combine(const std::u8string_view& path1, const std::u8string_view& path2, char8_t* destStr, int32_t destStrBufferLen);
    template <int32_t _Length>
    static std::optional<int32_t> Combine(const std::u8string_view& path1, const std::u8string_view& path2, const std::span<char8_t, _Length>& destStr);
    [[nodiscard]] static constexpr bool IsPathRooted(const std::u8string_view& path) noexcept;
    [[nodiscard]] static constexpr std::u8string_view GetExtension(const std::u8string_view& path) noexcept;
    [[nodiscard]] static constexpr std::u8string_view GetFileName(const std::u8string_view& path) noexcept;
    [[nodiscard]] static constexpr std::u8string_view GetFileNameWithoutExtension(const std::u8string_view& path) noexcept;
    [[nodiscard]] static constexpr std::u8string_view GetDirectoryName(const std::u8string_view& path) noexcept;
    [[nodiscard]] static constexpr bool HasExtension(const std::u8string_view& path) noexcept;
    [[nodiscard]] static std::u8string ChangeExtension(const std::u8string_view& path, const std::u8string_view& extension);
    static std::optional<int32_t> ChangeExtension(const std::u8string_view& path, const std::u8string_view& extension, char8_t* destStr, int32_t destStrBufferLen);
    [[nodiscard]] static std::u8string GetFullPath(const std::u8string_view& path);
    [[nodiscard]] static std::u8string GetFullPath(const std::u8string_view& path, const std::u8string_view& basePath);
    [[nodiscard]] static constexpr std::u8string_view GetPathRoot(const std::u8string_view& path) noexcept;
    static std::optional<int32_t> GetRandomFileName(char8_t* destStr, int32_t destStrBufferLen);
    template <int32_t _Length>
    static std::optional<int32_t> GetRandomFileName(const std::span<char8_t, _Length>& destStr);
    [[nodiscard]] static std::u8string GetRandomFileName();
    [[nodiscard]] static std::u8string GetTempPath();
    static std::optional<int32_t> GetTempPath(char8_t* destStr, int32_t destStrBufferLen);
    template <int32_t _Length>
    static std::optional<int32_t> GetTempPath(const std::span<char8_t, _Length>& destStr);
    [[nodiscard]] static std::span<const char8_t> GetInvalidFileNameChars() noexcept;
    [[nodiscard]] static std::span<const char8_t> GetInvalidPathChars() noexcept;
    [[nodiscard]] static constexpr int32_t GetRootLength(const std::u8string_view& path) noexcept;
    [[nodiscard]] static constexpr bool IsDirectorySeparator(char8_t c) noexcept;
    
private:
    static constexpr bool IsValidDriveChar(char8_t c) noexcept;
    static std::u8string RemoveRelativeSegments(const std::u8string_view& path);
    
/**@section Variable */
public:
    static constexpr char8_t AltDirectorySeparatorChar = PlatformPath::AltDirectorySeparatorChar;
    static constexpr char8_t DirectorySeparatorChar = PlatformPath::DirectorySeparatorChar;
    static constexpr char8_t PathSeparator = PlatformPath::PathSeparator;
    static constexpr char8_t VolumeSeparatorChar = PlatformPath::VolumeSeparatorChar;
    
private:
    static constexpr char8_t AltDirectorySeparatorStr[] = {AltDirectorySeparatorChar, u8'\0'};
    static constexpr char8_t DirectorySeparatorStr[] = {DirectorySeparatorChar, u8'\0'};
};

template <int32_t _Length>
std::optional<int32_t> Path::Combine(const std::u8string_view& path1, const std::u8string_view& path2, const std::span<char8_t, _Length>& destStr)
{
    return Combine(path1, path2, &destStr[0], _Length);
}

constexpr bool Path::IsPathRooted(const std::u8string_view& path) noexcept
{
    return (path.length() >= 1 && IsDirectorySeparator(path[0])) || (path.length() >= 2 && IsValidDriveChar(path[0]) && path[1] == VolumeSeparatorChar);
}

constexpr std::u8string_view Path::GetExtension(const std::u8string_view& path) noexcept
{
    auto index = path.length();
    while (index-- > 0)
    {
        if (path[index] == '.')
        {
            return path.substr(index);
        }
    }
    
    return {};
}

constexpr std::u8string_view Path::GetFileName(const std::u8string_view& path) noexcept
{
    auto index = path.length();
    while (index-- > 0)
    {
        if (path[index] == AltDirectorySeparatorChar || path[index] == DirectorySeparatorChar)
        {
            return path.substr(index + 1, path.length() - index);
        }
    }

    return path;
}

constexpr std::u8string_view Path::GetFileNameWithoutExtension(const std::u8string_view& path) noexcept
{
    auto extensionStartIndex = path.length();
    
    auto index = extensionStartIndex;
    while (index-- > 0)
    {
        if (IsDirectorySeparator(path[index]))
        {
            return path.substr(index + 1, extensionStartIndex - (index + 1));
        }
        else if (index == 0)
        {
            return path.substr(0, extensionStartIndex);
        }

        if (path[index] == '.' && (extensionStartIndex == path.length()))
        {
            extensionStartIndex = index;
        }
    }

    return path;
}

constexpr std::u8string_view Path::GetDirectoryName(const std::u8string_view& path) noexcept
{
    auto index = path.length();
    while (index-- > 0)
    {
        if (path[index] == AltDirectorySeparatorChar || path[index] == DirectorySeparatorChar)
        {
            return path.substr(0, index);
        }
    }

    return {};
}

constexpr bool Path::HasExtension(const std::u8string_view& path) noexcept
{
    auto index = path.length();
    while (index-- > 0)
    {
        if (path[index] == '.')
        {
            return path.length() > (index + 1);
        }
    }
    
    return false;
}

template <int32_t _Length>
std::optional<int32_t> Path::GetRandomFileName(const std::span<char8_t, _Length>& destStr)
{
    return GetRandomFileName(&destStr[0], _Length);
}

template <int32_t _Length>
std::optional<int32_t> Path::GetTempPath(const std::span<char8_t, _Length>& destStr)
{
    return GetTempPath(&destStr[0], _Length);
}

constexpr bool Path::IsDirectorySeparator(char8_t c) noexcept
{
    return c == AltDirectorySeparatorChar || c == DirectorySeparatorChar;
}

constexpr bool Path::IsValidDriveChar(char8_t c) noexcept
{
    return (c >= 'A' && c <= 'Z') || (c >= 'a' && c <= 'z');
}

}

#if TGON_PLATFORM_WINDOWS
#include "Windows/WindowsPath.inl"
#elif TGON_SUPPORT_POSIX
#include "Posix/PosixPath.inl"
#endif
