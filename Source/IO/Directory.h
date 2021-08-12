#pragma once

#include <optional>
#include <span>
#include <string>
#include <vector>

#include "DirectoryInfo.h"
#include "File.h"
#include "FileSystemEnumerable.h"

namespace tg
{

class Directory final
{
/**@section Constructor */
public:
    Directory() = delete;
    
/**@section Method */
public:
    [[nodiscard]] static DirectoryInfo CreateDirectory(const std::u8string_view& path);
    static bool Delete(const char8_t* path, bool recursive = false);
    static bool Exists(const char8_t* path);
    static bool Move(const char8_t* srcPath, const char8_t* destPath);
    static bool SetCurrentDirectory(const char8_t* path);
    static bool SetCreationTime(const char8_t* path, const DateTime& creationTime);
    static bool SetCreationTimeUtc(const char8_t* path, const DateTime& creationTimeUtc);
    static bool SetLastAccessTime(const char8_t* path, const DateTime& lastAccessTime);
    static bool SetLastAccessTimeUtc(const char8_t* path, const DateTime& lastAccessTimeUtc);
    static bool SetLastWriteTime(const char8_t* path, const DateTime& lastWriteTime);
    static bool SetLastWriteTimeUtc(const char8_t* path, const DateTime& lastWriteTimeUtc);
    [[nodiscard]] static std::optional<DateTime> GetCreationTime(const char8_t* path);
    [[nodiscard]] static std::optional<DateTime> GetCreationTimeUtc(const char8_t* path);
    [[nodiscard]] static std::optional<DateTime> GetLastAccessTime(const char8_t* path);
    [[nodiscard]] static std::optional<DateTime> GetLastAccessTimeUtc(const char8_t* path);
    [[nodiscard]] static std::optional<DateTime> GetLastWriteTime(const char8_t* path);
    [[nodiscard]] static std::optional<DateTime> GetLastWriteTimeUtc(const char8_t* path);
    [[nodiscard]] static std::u8string GetCurrentDirectory();
    static std::optional<int32_t> GetCurrentDirectory(char8_t* destStr, int32_t destStrBufferLen);
    template <int32_t _Length>
    static std::optional<int32_t> GetCurrentDirectory(const std::span<char8_t, _Length>& destStr);
    [[nodiscard]] static std::u8string GetDirectoryRoot(const std::u8string_view& path);
    [[nodiscard]] static std::vector<std::u8string> GetLogicalDrives();
    [[nodiscard]] static DirectoryInfo GetParent(const std::u8string_view& path);
    [[nodiscard]] static std::vector<std::u8string> GetDirectories(const char8_t* path, const char8_t* searchPattern = u8"*", SearchOption searchOption = SearchOption::TopDirectoryOnly);
    [[nodiscard]] static std::vector<std::u8string> GetFiles(const char8_t* path, const char8_t* searchPattern = u8"*", SearchOption searchOption = SearchOption::TopDirectoryOnly);
    [[nodiscard]] static std::vector<std::u8string> GetFileSystemEntries(const char8_t* path, const char8_t* searchPattern = u8"*", SearchOption searchOption = SearchOption::TopDirectoryOnly);
    template <typename _Predicate>
    static void EnumerateDirectories(const char8_t* path, const _Predicate& callback);
    template <typename _Predicate>
    static void EnumerateDirectories(const char8_t* path, const char8_t* searchPattern, const _Predicate& callback);
    template <typename _Predicate>
    static void EnumerateDirectories(const char8_t* path, const char8_t* searchPattern, SearchOption searchOption, const _Predicate& callback);
    template <typename _Predicate>
    static void EnumerateFiles(const char8_t* path, const _Predicate& callback);
    template <typename _Predicate>
    static void EnumerateFiles(const char8_t* path, const char8_t* searchPattern, const _Predicate& callback);
    template <typename _Predicate>
    static void EnumerateFiles(const char8_t* path, const char8_t* searchPattern, SearchOption searchOption, const _Predicate& callback);
    template <typename _Predicate>
    static void EnumerateFileSystemEntries(const char8_t* path, const _Predicate& callback);
    template <typename _Predicate>
    static void EnumerateFileSystemEntries(const char8_t* path, const char8_t* searchPattern, const _Predicate& callback);
    template <typename _Predicate>
    static void EnumerateFileSystemEntries(const char8_t* path, const char8_t* searchPattern, SearchOption searchOption, const _Predicate& callback);
    
private:
    static bool InternalCreateDirectory(const char8_t* path);
};

template <int32_t _Length>
std::optional<int32_t> Directory::GetCurrentDirectory(const std::span<char8_t, _Length>& destStr)
{
    return GetCurrentDirectory(destStr.data(), _Length);
}

template <typename _Predicate>
void Directory::EnumerateDirectories(const char8_t* path, const _Predicate& callback)
{
    return FileSystemEnumerable::EnumerateDirectories(path, callback);
}

template <typename _Predicate>
void Directory::EnumerateDirectories(const char8_t* path, const char8_t* searchPattern, const _Predicate& callback)
{
    return FileSystemEnumerable::EnumerateDirectories(path, searchPattern, callback);
}

template <typename _Predicate>
void Directory::EnumerateDirectories(const char8_t* path, const char8_t* searchPattern, SearchOption searchOption, const _Predicate& callback)
{
    return FileSystemEnumerable::EnumerateDirectories(path, searchPattern, searchOption, callback);
}

template <typename _Predicate>
void Directory::EnumerateFiles(const char8_t* path, const _Predicate& callback)
{
    return FileSystemEnumerable::EnumerateFiles(path, callback);
}

template <typename _Predicate>
void Directory::EnumerateFiles(const char8_t* path, const char8_t* searchPattern, const _Predicate& callback)
{
    return FileSystemEnumerable::EnumerateFiles(path, searchPattern, callback);
}

template <typename _Predicate>
void Directory::EnumerateFiles(const char8_t* path, const char8_t* searchPattern, SearchOption searchOption, const _Predicate& callback)
{
    return FileSystemEnumerable::EnumerateFiles(path, searchPattern, searchOption, callback);
}

template <typename _Predicate>
void Directory::EnumerateFileSystemEntries(const char8_t* path, const _Predicate& callback)
{
    return FileSystemEnumerable::EnumerateFileSystemEntries(path, callback);
}

template <typename _Predicate>
void Directory::EnumerateFileSystemEntries(const char8_t* path, const char8_t* searchPattern, const _Predicate& callback)
{
    return FileSystemEnumerable::EnumerateFileSystemEntries(path, searchPattern, callback);
}

template <typename _Predicate>
void Directory::EnumerateFileSystemEntries(const char8_t* path, const char8_t* searchPattern, SearchOption searchOption, const _Predicate& callback)
{
    return FileSystemEnumerable::EnumerateFileSystemEntries(path, searchPattern, searchOption, callback);
}

}
