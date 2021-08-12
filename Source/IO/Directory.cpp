#include "PrecompiledHeader.h"

#include <array>

#include "Text/FixedString.h"

#include "Directory.h"
#include "DirectoryInfo.h"

namespace tg
{

extern thread_local std::array<char8_t, 32768> g_tempUtf8StrBuffer;

DirectoryInfo Directory::CreateDirectory(const std::u8string_view& path)
{
    auto fullPath = Path::GetFullPath(path);
    if (fullPath.length() > 0 && Path::IsDirectorySeparator(fullPath.back()) == false)
    {
        fullPath += Path::DirectorySeparatorChar;
    }
    
    for (size_t i = Path::IsDirectorySeparator(fullPath.front()) ? 1 : 0; i < fullPath.length(); ++i)
    {
        const auto c = fullPath[i];
        if (Path::IsDirectorySeparator(c))
        {
            fullPath[i] = 0;
            InternalCreateDirectory(fullPath.data());
            
            fullPath[i] = c;
        }
    }
    
    return DirectoryInfo(fullPath, FullPathTag{});
}

DirectoryInfo Directory::GetParent(const std::u8string_view& path)
{
    U8FixedString8192 str(path);
    if (path.length() > 0)
    {
        if (Path::IsDirectorySeparator(str[str.Length() - 1]) == false)
        {
            str += Path::DirectorySeparatorChar;
        }
    }
    else
    {
        return DirectoryInfo(u8"", FullPathTag{});
    }
    
    str += u8"../";
    
    return DirectoryInfo({str.Data(), static_cast<size_t>(str.Length())});
}

std::u8string Directory::GetCurrentDirectory()
{
    auto strLen = GetCurrentDirectory(g_tempUtf8StrBuffer.data(), static_cast<int32_t>(g_tempUtf8StrBuffer.size()));
    return {g_tempUtf8StrBuffer.data(), static_cast<size_t>(*strLen)};
}

bool Directory::SetCreationTime(const char8_t* path, const DateTime& creationTime)
{
    return File::SetCreationTime(path, creationTime);
}

bool Directory::SetCreationTimeUtc(const char8_t* path, const DateTime& creationTimeUtc)
{
    return File::SetCreationTimeUtc(path, creationTimeUtc);
}

bool Directory::SetLastAccessTime(const char8_t* path, const DateTime& lastAccessTime)
{
    return File::SetLastAccessTime(path, lastAccessTime);
}

bool Directory::SetLastAccessTimeUtc(const char8_t* path, const DateTime& lastAccessTimeUtc)
{
    return File::SetLastAccessTimeUtc(path, lastAccessTimeUtc);
}

bool Directory::SetLastWriteTime(const char8_t* path, const DateTime& lastWriteTime)
{
    return File::SetLastWriteTime(path, lastWriteTime);
}

bool Directory::SetLastWriteTimeUtc(const char8_t* path, const DateTime& lastWriteTimeUtc)
{
    return File::SetLastWriteTimeUtc(path, lastWriteTimeUtc);
}

std::optional<DateTime> Directory::GetCreationTime(const char8_t* path)
{
    return File::GetCreationTime(path);
}

std::optional<DateTime> Directory::GetCreationTimeUtc(const char8_t* path)
{
    return File::GetCreationTimeUtc(path);
}

std::optional<DateTime> Directory::GetLastAccessTime(const char8_t* path)
{
    return File::GetLastAccessTime(path);
}

std::optional<DateTime> Directory::GetLastAccessTimeUtc(const char8_t* path)
{
    return File::GetLastAccessTimeUtc(path);
}

std::optional<DateTime> Directory::GetLastWriteTime(const char8_t* path)
{
    return File::GetLastWriteTime(path);
}

std::optional<DateTime> Directory::GetLastWriteTimeUtc(const char8_t* path)
{
    return File::GetLastWriteTimeUtc(path);
}

std::u8string Directory::GetDirectoryRoot(const std::u8string_view& path)
{
    if (path.length() <= 0)
    {
        return {};
    }
    
    const auto fullPath = Path::GetFullPath(path);
    return fullPath.substr(0, Path::GetRootLength(fullPath));
}

std::vector<std::u8string> Directory::GetDirectories(const char8_t* path, const char8_t* searchPattern, SearchOption searchOption)
{
    std::vector<std::u8string> ret;
    EnumerateDirectories(path, searchPattern, searchOption, [&](const std::u8string_view& str)
    {
        ret.emplace_back(str);
    });

    return ret;
}

std::vector<std::u8string> Directory::GetFiles(const char8_t* path, const char8_t* searchPattern, SearchOption searchOption)
{
    std::vector<std::u8string> ret;
    EnumerateFiles(path, searchPattern, searchOption, [&](const std::u8string_view& str)
    {
        ret.emplace_back(str);
    });

    return ret;
}

std::vector<std::u8string> Directory::GetFileSystemEntries(const char8_t* path, const char8_t* searchPattern, SearchOption searchOption)
{
    std::vector<std::u8string> ret;
    EnumerateFileSystemEntries(path, searchPattern, searchOption, [&](const std::u8string_view& str)
    {
        ret.emplace_back(str);
    });

    return ret;
}

}
