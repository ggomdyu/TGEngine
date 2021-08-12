#include "PrecompiledHeader.h"

#include <sys/stat.h>

#include "../File.h"

namespace tg
{

bool File::Delete(const char8_t* path)
{
    struct stat s;
    if (stat(reinterpret_cast<const char*>(path), &s) != 0 || S_ISREG(s.st_mode) == false)
    {
        return false;
    }
    
    return remove(reinterpret_cast<const char*>(path)) == 0;
}

bool File::Exists(const char8_t* path)
{
    struct stat s;
    if (stat(reinterpret_cast<const char*>(path), &s) != 0 || S_ISREG(s.st_mode) == false)
    {
        return false;
    }
    
    return true;
}

bool File::Move(const char8_t* srcPath, const char8_t* destPath)
{
    struct stat s;
    if (stat(reinterpret_cast<const char*>(srcPath), &s) != 0 || S_ISREG(s.st_mode) == false)
    {
        return false;
    }
    
    return rename(reinterpret_cast<const char*>(srcPath), reinterpret_cast<const char*>(destPath)) == 0;
}

std::optional<DateTime> File::GetCreationTimeUtc(const char8_t* path)
{
    struct stat s;
    if (stat(reinterpret_cast<const char*>(path), &s) != 0)
    {
        return {};
    }

#if TGON_PLATFORM_MACOS
    return DateTime(DateTime::GetUnixEpoch().GetTicks() + TimeSpan::TicksPerSecond * s.st_birthtimespec.tv_sec);
#else
    return DateTime(DateTime::GetUnixEpoch().GetTicks() + TimeSpan::TicksPerSecond * s->st_ctime);
#endif
}

std::optional<DateTime> File::GetLastAccessTimeUtc(const char8_t* path)
{
    struct stat s;
    if (stat(reinterpret_cast<const char*>(path), &s) != 0)
    {
        return {};
    }

#if TGON_PLATFORM_MACOS
    return DateTime(DateTime::GetUnixEpoch().GetTicks() + TimeSpan::TicksPerSecond * s.st_atimespec.tv_sec);
#else
    return DateTime(DateTime::GetUnixEpoch().GetTicks() + TimeSpan::TicksPerSecond * s.st_atime);
#endif
}

std::optional<DateTime> File::GetLastWriteTimeUtc(const char8_t* path)
{
    struct stat s;
    if (stat(reinterpret_cast<const char*>(path), &s) != 0)
    {
        return {};
    }

#if TGON_PLATFORM_MACOS
    return DateTime(DateTime::GetUnixEpoch().GetTicks() + TimeSpan::TicksPerSecond * s.st_mtimespec.tv_sec);
#else
    return DateTime(DateTime::GetUnixEpoch().GetTicks() + TimeSpan::TicksPerSecond * s.st_mtime);
#endif
}

}
