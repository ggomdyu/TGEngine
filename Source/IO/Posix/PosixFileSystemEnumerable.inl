#pragma once

#include <array>
#include <deque>
#include <dirent.h>
#include <fnmatch.h>
#include <unistd.h>

#include "Core/TypeTraits.h"

#include "../Path.h"

namespace tg
{
namespace detail
{

template <typename _Predicate>
void InternalEnumerateAllDirectories(const char8_t* path, const char8_t* searchPattern, uint8_t filterType, const _Predicate& handler)
{
    std::deque<std::u8string> directories(1, std::u8string(path));
    
    while (directories.empty() == false)
    {
        const std::u8string& currPath = directories.front();
        
        DIR* dir = opendir(reinterpret_cast<const char*>(currPath.data()));
        if (dir != nullptr)
        {
            struct dirent* ent;
            while ((ent = readdir(dir)) != nullptr)
            {   
                if (ent->d_type & DT_DIR)
                {
                    // Ignore the . and ..
                    if ((ent->d_namlen == 1 && ent->d_name[0] == '.') || (ent->d_namlen == 2 && !strcmp(ent->d_name, "..")))
                    {
                        continue;
                    }
                    
                    directories.push_back(Path::Combine(currPath, {reinterpret_cast<const char8_t*>(ent->d_name), ent->d_namlen}));
                }
                
                if (ent->d_type & filterType)
                {
                    // Check the file name matched with wildcards
                    if (fnmatch(reinterpret_cast<const char*>(searchPattern), ent->d_name, FNM_PATHNAME) != 0)
                    {
                        continue;
                    }
                    
                    auto newPath = Path::Combine(currPath, {reinterpret_cast<const char8_t*>(ent->d_name), ent->d_namlen});
                    if constexpr (std::is_same_v<typename FunctionTraits<_Predicate>::ReturnType, bool>)
                    {
                        if (handler(std::move(newPath)) == false)
                        {
                            return;
                        }
                    }
                    else
                    {
                        handler(std::move(newPath));
                    }
                }
            }
            closedir(dir);
        }
        
        directories.pop_front();
    }
}

template <typename _Predicate>
void InternalEnumerateTopDirectoryOnly(const char8_t* path, const char8_t* searchPattern, uint8_t filterType, const _Predicate& handler)
{
    DIR* dir = opendir(reinterpret_cast<const char*>(path));
    if (dir != nullptr)
    {
        struct dirent* ent;
        while ((ent = readdir(dir)) != nullptr)
        {
            if (ent->d_type & filterType)
            {
                // Ignore the . and ..
                if ((ent->d_namlen == 1 && ent->d_name[0] == '.') || (ent->d_namlen == 2 && !strcmp(ent->d_name, "..")))
                {
                    continue;
                }
                
                // Check the file name matched with wildcards
                if (fnmatch(reinterpret_cast<const char*>(searchPattern), reinterpret_cast<const char*>(ent->d_name), FNM_PATHNAME) != 0)
                {
                    continue;
                }
                
                auto newPath = Path::Combine(path, {reinterpret_cast<const char8_t*>(ent->d_name), ent->d_namlen});
                if constexpr (std::is_same_v<typename FunctionTraits<_Predicate>::ReturnType, bool>)
                {
                    if (handler(std::move(newPath)) == false)
                    {
                        break;
                    }
                }
                else
                {
                    handler(std::move(newPath));
                }
            }
        }
        closedir(dir);
    }
}

}

template <typename _Predicate>
void FileSystemEnumerable::EnumerateDirectories(const char8_t* path, const char8_t* searchPattern, SearchOption searchOption, const _Predicate& handler)
{
    if (searchOption == SearchOption::AllDirectories)
    {
        detail::InternalEnumerateAllDirectories(path, searchPattern, DT_DIR, handler);
    }
    else
    {
        detail::InternalEnumerateTopDirectoryOnly(path, searchPattern, DT_DIR, handler);
    }
}

template <typename _Predicate>
void FileSystemEnumerable::EnumerateFiles(const char8_t* path, const char8_t* searchPattern, SearchOption searchOption, const _Predicate& handler)
{
    if (searchOption == SearchOption::AllDirectories)
    {
        detail::InternalEnumerateAllDirectories(path, searchPattern, DT_REG, handler);
    }
    else
    {
        detail::InternalEnumerateTopDirectoryOnly(path, searchPattern, DT_REG, handler);
    }
}

template <typename _Predicate>
void FileSystemEnumerable::EnumerateFileSystemEntries(const char8_t* path, const char8_t* searchPattern, SearchOption searchOption, const _Predicate& handler)
{
    if (searchOption == SearchOption::AllDirectories)
    {
        detail::InternalEnumerateAllDirectories(path, searchPattern, DT_DIR | DT_REG, handler);
    }
    else
    {
        detail::InternalEnumerateTopDirectoryOnly(path, searchPattern, DT_DIR | DT_REG, handler);
    }
}

}
