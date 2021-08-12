#include "PrecompiledHeader.h"

#include <array>
#include <sys/stat.h>

#include "Platform/Windows/SafeFindHandle.h"
#include "Platform/Windows/Windows.h"

#include "../Directory.h"
#include "../Path.h"

namespace tg
{

extern thread_local std::array<wchar_t, 32768> g_tempUtf16StrBuffer;

namespace
{

#if !defined(S_ISDIR)
constexpr bool S_ISDIR(unsigned short m) noexcept
{
    return (m & S_IFMT) == S_IFDIR;
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

bool InternalRecursiveDelete(const std::wstring_view& path)
{
    memcpy(g_tempUtf16StrBuffer.data(), path.data(), path.size() * 2);

    auto pathLen = path.size();
    if (Path::IsDirectorySeparator(static_cast<char8_t>(path.back())))
    {
        g_tempUtf16StrBuffer[pathLen] = L'*';
        pathLen += 1;
    }
    else
    {
        g_tempUtf16StrBuffer[pathLen] = Path::DirectorySeparatorChar;
        g_tempUtf16StrBuffer[pathLen + 1] = L'*';
        pathLen += 2;
    }

    WIN32_FIND_DATAW findData;
    const SafeFindHandle handle(FindFirstFileW(g_tempUtf16StrBuffer.data(), &findData));
    if (handle == nullptr)
    {
        return false;
    }

    do
    {
        if (findData.dwFileAttributes & FILE_ATTRIBUTE_DIRECTORY)
        {
            // Ignore the . and ..
            if (findData.cFileName[0] == L'.' && (findData.cFileName[1] == L'\0' || (findData.cFileName[1] == L'.' && findData.cFileName[2] == L'\0')))
            {
                continue;
            }

            const auto fileNameLen = std::char_traits<wchar_t>::length(findData.cFileName);
            memcpy(&g_tempUtf16StrBuffer[pathLen - 1], findData.cFileName, fileNameLen * 2 + sizeof(findData.cFileName[0]));

            InternalRecursiveDelete({g_tempUtf16StrBuffer.data(), pathLen + fileNameLen - 1});
        }
        else
        {
            const auto fileNameLen = std::char_traits<wchar_t>::length(findData.cFileName);
            memcpy(&g_tempUtf16StrBuffer[pathLen - 1], findData.cFileName, fileNameLen * 2 + sizeof(findData.cFileName[0]));

            _wremove(g_tempUtf16StrBuffer.data());
        }
    } while (FindNextFileW(handle, &findData) == TRUE);

    return _wrmdir(path.data()) == 0;
}

}

bool Directory::Delete(const char8_t* path, bool recursive)
{
    auto s = CreateStat(path, g_tempUtf16StrBuffer);
    if (s.has_value() == false || S_ISDIR(s->st_mode) == false)
    {
        return false;
    }

    if (recursive)
    {
        return InternalRecursiveDelete(g_tempUtf16StrBuffer.data());
    }

    return _wrmdir(g_tempUtf16StrBuffer.data()) == 0;
}

bool Directory::Exists(const char8_t* path)
{
    auto s = CreateStat(path, g_tempUtf16StrBuffer);
    if (s.has_value() == false || S_ISDIR(s->st_mode) == false)
    {
        return false;
    }

    return true;
}

std::vector<std::u8string> Directory::GetLogicalDrives()
{
    const auto driveFlags = ::GetLogicalDrives();

    std::vector<std::u8string> ret;
    char8_t root[] = u8"A:\\";
    for (auto d = driveFlags; d != 0; d >>= 1)
    {
        if ((d & 1) != 0)
        {
            ret.emplace_back(root, std::extent_v<decltype(root)> - 1);
        }

        ++root[0];
    }

    return ret;
}

bool Directory::Move(const char8_t* srcPath, const char8_t* destPath)
{
    const auto utf16SrcPath = std::span(g_tempUtf16StrBuffer).subspan(0, g_tempUtf16StrBuffer.size() / 2);
    auto s = CreateStat(srcPath, utf16SrcPath);
    if (s.has_value() == false || S_ISDIR(s->st_mode) == false)
    {
        return false;
    }

    const auto utf16DestPath = std::span(g_tempUtf16StrBuffer).subspan(g_tempUtf16StrBuffer.size() / 2, g_tempUtf16StrBuffer.size() / 2);
    if (MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(destPath), -1, &utf16DestPath[0], static_cast<int>(utf16DestPath.size())) == 0)
    {
        return {};
    }

    return _wrename(utf16SrcPath.data(), utf16DestPath.data()) == 0;
}

bool Directory::SetCurrentDirectory(const char8_t* path)
{
    if (MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(path), -1, g_tempUtf16StrBuffer.data(), g_tempUtf16StrBuffer.size()) == 0)
    {
        return false;
    }

    return _wchdir(g_tempUtf16StrBuffer.data()) == 0;
}

std::optional<int32_t> Directory::GetCurrentDirectory(char8_t* destStr, int32_t destStrBufferLen)
{
    if (GetCurrentDirectoryW(static_cast<DWORD>(g_tempUtf16StrBuffer.size()), g_tempUtf16StrBuffer.data()) == 0)
    {
        return {};
    }

    const auto utf8PathLen = WideCharToMultiByte(CP_UTF8, 0, g_tempUtf16StrBuffer.data(), -1, reinterpret_cast<char*>(destStr), destStrBufferLen, nullptr, nullptr) - 1;
    if (utf8PathLen == -1)
    {
        return {};
    }

    return utf8PathLen;
}

bool Directory::InternalCreateDirectory(const char8_t* path)
{
    if (MultiByteToWideChar(CP_UTF8, 0, reinterpret_cast<const char*>(path), -1, g_tempUtf16StrBuffer.data(), g_tempUtf16StrBuffer.size()) == 0)
    {
        return false;
    }

    return _wmkdir(g_tempUtf16StrBuffer.data()) == 0;
}

}