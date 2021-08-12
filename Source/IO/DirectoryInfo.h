#pragma once

#include <string>
#include <vector>

#include "FileSystemEnumerable.h"

namespace tg
{

struct FullPathTag {};

class DirectoryInfo final
{
/**@section Constructor */
public:
    explicit DirectoryInfo(const std::u8string_view& path);
    DirectoryInfo(std::u8string path, FullPathTag);
    
/**@section Method */
public:
    void Create() const;
    [[nodiscard]] DirectoryInfo CreateSubdirectory(const std::u8string_view& path) const;
    bool Delete(bool recursive = false) const;
    [[nodiscard]] bool IsExists() const;
    bool MoveTo(const char8_t* destDirName) const;
    [[nodiscard]] std::u8string_view GetName() const noexcept;
    [[nodiscard]] DirectoryInfo GetRoot() const;
    [[nodiscard]] DirectoryInfo GetParent() const;
    [[nodiscard]] std::vector<std::u8string> GetDirectories(const char8_t* searchPattern = u8"*", SearchOption searchOption = SearchOption::TopDirectoryOnly) const;
    [[nodiscard]] std::vector<std::u8string> GetFiles(const char8_t* searchPattern = u8"*", SearchOption searchOption = SearchOption::TopDirectoryOnly) const;
    [[nodiscard]] std::vector<std::u8string> GetFileSystemEntries(const char8_t* searchPattern = u8"*", SearchOption searchOption = SearchOption::TopDirectoryOnly) const;
    template <typename _Predicate>
    void EnumerateDirectories(const _Predicate& callback) const;
    template <typename _Predicate>
    void EnumerateDirectories(const char8_t* searchPattern, const _Predicate& callback) const;
    template <typename _Predicate>
    void EnumerateDirectories(const char8_t* searchPattern, SearchOption searchOption, const _Predicate& callback) const;
    template <typename _Predicate>
    void EnumerateFiles(const _Predicate& callback) const;
    template <typename _Predicate>
    void EnumerateFiles(const char8_t* searchPattern, const _Predicate& callback) const;
    template <typename _Predicate>
    void EnumerateFiles(const char8_t* searchPattern, SearchOption searchOption, const _Predicate& callback) const;
    template <typename _Predicate>
    void EnumerateFileSystemEntries(const _Predicate& callback);
    template <typename _Predicate>
    void EnumerateFileSystemEntries(const char8_t* searchPattern, const _Predicate& callback);
    template <typename _Predicate>
    void EnumerateFileSystemEntries(const char8_t* searchPattern, SearchOption searchOption, const _Predicate& callback);
    [[nodiscard]] const std::u8string& ToString() const noexcept;
    
/**@section Variable */
private:
    std::u8string m_fullPath;
};

template <typename _Predicate>
void DirectoryInfo::EnumerateDirectories(const _Predicate& callback) const
{
    FileSystemEnumerable::EnumerateDirectories(m_fullPath.c_str(), callback);
}

template <typename _Predicate>
void DirectoryInfo::EnumerateDirectories(const char8_t* searchPattern, const _Predicate& callback) const
{
    FileSystemEnumerable::EnumerateDirectories(m_fullPath.c_str(), searchPattern, callback);
}

template <typename _Predicate>
void DirectoryInfo::EnumerateDirectories(const char8_t* searchPattern, SearchOption searchOption, const _Predicate& callback) const
{
    FileSystemEnumerable::EnumerateDirectories(m_fullPath.c_str(), searchPattern, searchOption, callback);
}

template <typename _Predicate>
void DirectoryInfo::EnumerateFiles(const _Predicate& callback) const
{
    FileSystemEnumerable::EnumerateFiles(m_fullPath.c_str(), callback);
}

template <typename _Predicate>
void DirectoryInfo::EnumerateFiles(const char8_t* searchPattern, const _Predicate& callback) const
{
    FileSystemEnumerable::EnumerateFiles(m_fullPath.c_str(), searchPattern, callback);
}

template <typename _Predicate>
void DirectoryInfo::EnumerateFiles(const char8_t* searchPattern, SearchOption searchOption, const _Predicate& callback) const
{
    FileSystemEnumerable::EnumerateFiles(m_fullPath.c_str(), searchPattern, searchOption, callback);
}

template <typename _Predicate>
void DirectoryInfo::EnumerateFileSystemEntries(const _Predicate& callback)
{
    FileSystemEnumerable::EnumerateFileSystemEntries(m_fullPath.c_str(), callback);
}

template <typename _Predicate>
void DirectoryInfo::EnumerateFileSystemEntries(const char8_t* searchPattern, const _Predicate& callback)
{
    FileSystemEnumerable::EnumerateFileSystemEntries(m_fullPath.c_str(), searchPattern, callback);
}

template <typename _Predicate>
void DirectoryInfo::EnumerateFileSystemEntries(const char8_t* searchPattern, SearchOption searchOption, const _Predicate& callback)
{
    FileSystemEnumerable::EnumerateFileSystemEntries(m_fullPath.c_str(), searchPattern, searchOption, callback);
}

}
