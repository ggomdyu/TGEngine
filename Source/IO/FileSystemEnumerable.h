#pragma once

namespace tg
{

enum class SearchOption
{
    TopDirectoryOnly,
    AllDirectories,
};

class FileSystemEnumerable
{
/**@section Constructor */
public:
    FileSystemEnumerable() = delete;
    
/**@section Method */
public:
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
};

template <typename _Predicate>
void FileSystemEnumerable::EnumerateDirectories(const char8_t* path, const _Predicate& callback)
{
    FileSystemEnumerable::EnumerateDirectories(path, u8"*", callback);
}

template <typename _Predicate>
void FileSystemEnumerable::EnumerateDirectories(const char8_t* path, const char8_t* searchPattern, const _Predicate& callback)
{
    FileSystemEnumerable::EnumerateDirectories(path, searchPattern, SearchOption::TopDirectoryOnly, callback);
}

template <typename _Predicate>
void FileSystemEnumerable::EnumerateFiles(const char8_t* path, const _Predicate& callback)
{
    FileSystemEnumerable::EnumerateFiles(path, u8"*", callback);
}

template <typename _Predicate>
void FileSystemEnumerable::EnumerateFiles(const char8_t* path, const char8_t* searchPattern, const _Predicate& callback)
{
    FileSystemEnumerable::EnumerateFiles(path, searchPattern, SearchOption::TopDirectoryOnly, callback);
}

template <typename _Predicate>
void FileSystemEnumerable::EnumerateFileSystemEntries(const char8_t* path, const _Predicate& callback)
{
    FileSystemEnumerable::EnumerateFileSystemEntries(path, u8"*", callback);
}

template <typename _Predicate>
void FileSystemEnumerable::EnumerateFileSystemEntries(const char8_t* path, const char8_t* searchPattern, const _Predicate& callback)
{
    FileSystemEnumerable::EnumerateFileSystemEntries(path, searchPattern, SearchOption::TopDirectoryOnly, callback);
}

}

#if TGON_PLATFORM_WINDOWS
#include "Windows/WindowsFileSystemEnumerable.inl"
#elif TGON_PLATFORM_MACOS
#include "Posix/PosixFileSystemEnumerable.inl"
#endif