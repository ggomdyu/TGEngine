#include "System.IO/Path.h"
#include "System.IO.Enumeration/DirectoryIterator.h"
#include "System.Text/Encoding.h"

TG_NAMESPACE_BEGIN

thread_local static std::u16string Buffer;

DirectoryIterator::DirectoryIterator(std::u16string_view basePath, std::u16string_view searchPattern, uint32_t filter) noexcept :
    dir_(OpenFindHandle(basePath)),
    basePath_(basePath),
    searchPattern_(reinterpret_cast<const wchar_t*>(searchPattern.data()), searchPattern.length()),
    filter_(filter)
{
}

void DirectoryIterator::Advance()
{
    while (true)
    {
        struct dirent* ent = readdir(dir_.get());
        if (!ent)
        {
            break;
        }

        if (!(ent->d_type & filter_))
        {
            continue;
        }

#if TG_PLATFORM_DARWIN
        std::string_view entryName(ent->d_name, ent->d_namlen);
#else
        std::string_view entryName(ent->d_name);
#endif
        // Ignore the . and ..
        if ((entryName.length() == 1 && entryName[0] == '.') ||
            (entryName.length() == 2 && !strcmp(entryName.data(), "..")))
        {
            continue;
        }

        // Ignore the file that the filename doesn't matched with wildcards.
        auto newPath = std::filesystem::path(path) /= entryName;
        if (fnmatch(searchPattern, newPath.c_str(), 0) != 0)
        {
            continue;
        }

        if constexpr (std::is_same_v<typename FunctionTraits<F>::Return, bool>)
        {
            if (!callback(std::move(newPath.u16string())))
            {
                break;
            }
        }
        else
        {
            callback(std::move(newPath.u16string()));
        }
    }

//    while (FindNextFileW(handle_, &findFileData_) == TRUE)
//    {
//        if (!(findFileData_.dwFileAttributes & filter_))
//        {
//            continue;
//        }
//
//        // Ignore the . and ..
//        if (findFileData_.cFileName[0] == L'.' &&
//           (findFileData_.cFileName[1] == L'\0' || (findFileData_.cFileName[1] == L'.' && findFileData_.cFileName[2] == L'\0')))
//        {
//            continue;
//        }
//
//        // Check whether the filename matches with the pattern or not.
//        if (!IsMatchWithPattern(findFileData_.cFileName, searchPattern_))
//        {
//            continue;
//        }
//
//        // Combine the current path and a filename.
//        Buffer = basePath_;
//        if (!Buffer.empty() && !(Path::IsDirectorySeparator(Buffer.back()) || Path::IsVolumeSeparator(Buffer.back())))
//        {
//            Buffer += Path::DirectorySeparatorChar;
//        }
//        Buffer += reinterpret_cast<const char16_t*>(findFileData_.cFileName);
//
//        break;
//    }
}

std::u16string_view DirectoryIterator::Entry() noexcept
{
    return Buffer;
}

std::unique_ptr<DIR, decltype(&closedir)> DirectoryIterator::OpenFindHandle(std::u16string_view basePath) noexcept
{
    auto utf8Path = Encoding::UTF8().GetBytes(basePath);
    if (!utf8Path)
    {
        return {nullptr, closedir};
    }

    return {opendir(reinterpret_cast<const char*>(utf8Path->data())), closedir};
}

TG_NAMESPACE_END