#include "PrecompiledHeader.h"

#include "Path.h"
#include "DirectoryInfo.h"
#include "Directory.h"

namespace tg
{

DirectoryInfo::DirectoryInfo(const std::u8string_view& path) :
    m_fullPath(Path::GetFullPath(path))
{
}

DirectoryInfo::DirectoryInfo(std::u8string path, FullPathTag) :
    m_fullPath(std::move(path))
{
}

bool DirectoryInfo::IsExists() const
{
    return Directory::Exists(m_fullPath.data());
}

std::u8string_view DirectoryInfo::GetName() const noexcept
{
    return Path::GetDirectoryName(m_fullPath);
}

DirectoryInfo DirectoryInfo::GetParent() const
{
    return Directory::GetParent(m_fullPath);
}

DirectoryInfo DirectoryInfo::GetRoot() const
{
    return DirectoryInfo(Directory::GetDirectoryRoot(m_fullPath), FullPathTag{});
}

std::vector<std::u8string> DirectoryInfo::GetDirectories(const char8_t* searchPattern, SearchOption searchOption) const
{
    return Directory::GetDirectories(m_fullPath.c_str(), searchPattern, searchOption);
}

std::vector<std::u8string> DirectoryInfo::GetFiles(const char8_t* searchPattern, SearchOption searchOption) const
{
    return Directory::GetFiles(m_fullPath.c_str(), searchPattern, searchOption);
}

std::vector<std::u8string> DirectoryInfo::GetFileSystemEntries(const char8_t* searchPattern, SearchOption searchOption) const
{
    return Directory::GetFileSystemEntries(m_fullPath.c_str(), searchPattern, searchOption);
}

void DirectoryInfo::Create() const
{
    Directory::CreateDirectory(m_fullPath.c_str());
}

DirectoryInfo DirectoryInfo::CreateSubdirectory(const std::u8string_view& path) const
{
    const auto combinedPath = Path::Combine(m_fullPath, path);
    return Directory::CreateDirectory(combinedPath.c_str());
}

bool DirectoryInfo::MoveTo(const char8_t* destDirName) const
{
    return Directory::Move(m_fullPath.c_str(), destDirName);
}

bool DirectoryInfo::Delete(bool recursive) const
{
    return Directory::Delete(m_fullPath.data(), recursive);
}

const std::u8string& DirectoryInfo::ToString() const noexcept
{
    return m_fullPath;
}

}
