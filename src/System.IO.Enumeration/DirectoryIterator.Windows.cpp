#include <Shlwapi.h>
#include <filesystem>

#include "System.IO.Enumeration/DirectoryIterator.h"
#include "System.IO/Path.h"

#pragma comment(lib, "Shlwapi.lib")

TG_NAMESPACE_BEGIN

static bool IsMatchWithPattern(std::wstring_view path, std::wstring_view searchPattern) noexcept
{
    if (searchPattern == L"*")
    {
        return true;
    }

    return PathMatchSpecW(path.data(), searchPattern.data()) == TRUE;
}

static std::unique_ptr<void, void(*)(void*)> OpenHandle(std::u16string_view basePath, std::u16string_view searchPattern, WIN32_FIND_DATAW& findFileData) noexcept
{
    auto path = Path::Combine(basePath, searchPattern);
    auto deleter = [](void* handle)
    {
      if (handle != INVALID_HANDLE_VALUE)
      {
          CloseHandle(handle);
      }
    };

    return {FindFirstFileW(reinterpret_cast<LPCWSTR>(path.c_str()), &findFileData), deleter};
}

DirectoryIterator::DirectoryIterator(std::u16string_view basePath, std::u16string_view searchPattern, uint32_t filter) noexcept :
    findData_{},
    handle_(OpenHandle(basePath, searchPattern, findData_)),
    basePath_(basePath),
    searchPattern_(reinterpret_cast<const wchar_t*>(searchPattern.data()), searchPattern.length()),
    filter_(filter)
{
}

void DirectoryIterator::Advance()
{
    while (FindNextFileW(handle_.get(), &findData_) == TRUE)
    {
        if (!(findData_.dwFileAttributes & filter_))
        {
            continue;
        }

        // Ignore the . and ..
        if (findData_.cFileName[0] == L'.' &&
           (findData_.cFileName[1] == L'\0' || (findData_.cFileName[1] == L'.' && findData_.cFileName[2] == L'\0')))
        {
            continue;
        }

        // Check whether the filename matches with the pattern or not.
        if (!IsMatchWithPattern(findData_.cFileName, searchPattern_))
        {
            continue;
        }
            
        // Combine the current path and the filename.
        buffer_ = basePath_;
        if (!buffer_.empty() && !(Path::IsDirectorySeparator(buffer_.back()) || Path::IsVolumeSeparator(buffer_.back())))
        {
            buffer_ += Path::DirectorySeparatorChar;
        }
        buffer_ += reinterpret_cast<const char16_t*>(findData_.cFileName);

        break;
    }
}

std::u16string_view DirectoryIterator::Entry() noexcept
{
    return buffer_;
}

TG_NAMESPACE_END