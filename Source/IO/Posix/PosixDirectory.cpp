#include "PrecompiledHeader.h"

#include <array>
#include <unistd.h>
#include <sys/stat.h>

#include "../Path.h"
#include "../DirectoryInfo.h"
#include "../Directory.h"

namespace tg
{
namespace
{

bool InternalRecursiveDelete(const std::u8string& path)
{
    DIR* dir = opendir(reinterpret_cast<const char*>(path.c_str()));
    if (dir == nullptr)
    {
        return false;
    }
    
    struct dirent* ent;
    while ((ent = readdir(dir)) != nullptr)
    {
        // Ignore the . and ..
        if ((ent->d_namlen == 1 && ent->d_name[0] == '.') || (ent->d_namlen == 2 && !strcmp(ent->d_name, "..")))
        {
            continue;
        }
        
        auto combinedPath = Path::Combine(path, {reinterpret_cast<const char8_t*>(ent->d_name), ent->d_namlen});
        if (ent->d_type & DT_DIR)
        {
            InternalRecursiveDelete(combinedPath);
        }
        else
        {
            unlink(reinterpret_cast<const char*>(combinedPath.c_str()));
        }
    }
    closedir(dir);

    return rmdir(reinterpret_cast<const char*>(path.c_str())) == 0;
}

}

bool Directory::Exists(const char8_t* path)
{
    struct stat s;
    if (stat(reinterpret_cast<const char*>(path), &s) != 0 || S_ISDIR(s.st_mode) == false)
    {
        return false;
    }
    
    return true;
}

bool Directory::Delete(const char8_t* path, bool recursive)
{
    if (recursive)
    {
        return InternalRecursiveDelete(path);
    }
    
    return rmdir(reinterpret_cast<const char*>(path)) == 0;
}

bool Directory::Move(const char8_t* srcPath, const char8_t* destPath)
{
    struct stat s;
    if (stat(reinterpret_cast<const char*>(srcPath), &s) != 0 || S_ISDIR(s.st_mode) == false)
    {
        return false;
    }
    
    return rename(reinterpret_cast<const char*>(srcPath), reinterpret_cast<const char*>(destPath)) == 0;
}

bool Directory::SetCurrentDirectory(const char8_t* path)
{
    return chdir(reinterpret_cast<const char*>(path)) == 0;
}

std::optional<int32_t> Directory::GetCurrentDirectory(char8_t* destStr, int32_t destStrBufferLen)
{
    if (getcwd(reinterpret_cast<char*>(destStr), destStrBufferLen) == nullptr)
    {
        return {};
    }

    return static_cast<int32_t>(std::char_traits<char8_t>::length(destStr));
}

std::vector<std::u8string> Directory::GetLogicalDrives()
{
    // TODO: Implement
    return {};
}

bool Directory::InternalCreateDirectory(const char8_t* path)
{
    return mkdir(reinterpret_cast<const char*>(path), 0777) == 0;
}

}

