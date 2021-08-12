#include "PrecompiledHeader.h"

#include <array>

#include "Platform/Environment.h"
#include "Random/Random.h"

#include "Directory.h"
#include "Path.h"

namespace tg
{

extern thread_local std::array<char8_t, 32768> g_tempUtf8StrBuffer;

std::u8string Path::Combine(const std::u8string_view& path1, const std::u8string_view& path2)
{
    const auto strLen = Combine(path1, path2, g_tempUtf8StrBuffer.data(), static_cast<int32_t>(g_tempUtf8StrBuffer.size()));
    return {g_tempUtf8StrBuffer.data(), static_cast<size_t>(*strLen)};
}

std::optional<int32_t> Path::Combine(const std::u8string_view& path1, const std::u8string_view& path2, char8_t* destStr, int32_t destStrBufferLen)
{
    if (path2.length() == 0)
    {
        const auto destStrLen = static_cast<int32_t>(path1.length());
        if (destStrLen + 1 > destStrBufferLen)
        {
            return {};
        }

        memcpy(destStr, path1.data(), static_cast<size_t>(destStrLen) + 1);
        return destStrLen;
    }

    if (path1.length() == 0 || IsPathRooted(path2))
    {
        if (static_cast<int32_t>(path2.length() + 1) > destStrBufferLen)
        {
            return {};
        }

        memcpy(destStr, path2.data(), path2.length() + 1);
        return static_cast<int32_t>(path2.length());
    }

    auto index = static_cast<int32_t>(path1.size());

    const auto path1Separator = path1[path1.length() - 1];
    const auto path2Separator = path2[0];
    const auto hasSeparator = (path1Separator == DirectorySeparatorChar) || (path1Separator == AltDirectorySeparatorChar) || (path2Separator == DirectorySeparatorChar) || (path2Separator == AltDirectorySeparatorChar);
    if (!hasSeparator)
    {
        if (index + 1 + static_cast<int32_t>(path2.length()) > destStrBufferLen)
        {
            return {};
        }

        destStr[index] = DirectorySeparatorChar;
        index += 1;
    }
    else
    {
        if (index + static_cast<int32_t>(path2.length()) > destStrBufferLen)
        {
            return {};
        }
    }
    
    memcpy(destStr, path1.data(), path1.length());
    memcpy(&destStr[index], path2.data(), path2.length() + 1);
    
    return index + static_cast<int32_t>(path2.length());
}

std::u8string Path::ChangeExtension(const std::u8string_view& path, const std::u8string_view& extension)
{
    const auto strLen = ChangeExtension(path, extension, g_tempUtf8StrBuffer.data(), static_cast<int32_t>(g_tempUtf8StrBuffer.size()));
    return {g_tempUtf8StrBuffer.data(), static_cast<size_t>(*strLen)};
}

std::optional<int32_t> Path::ChangeExtension(const std::u8string_view& path, const std::u8string_view& extension, char8_t* destStr, int32_t destStrBufferLen)
{
    if (path.length() == 0)
    {
        if (destStrBufferLen >= 1)
        {
            destStr[0] = '\0';
        }
        return {};
    }
    
    auto extensionStartIndex = static_cast<int32_t>(path.length());
    for (auto i = extensionStartIndex - 1; i >= 0; --i)
    {
        if (path[i] == '.')
        {
            extensionStartIndex = i;
            break;
        }
    }
    
    const auto extensionHasDot = extension.length() > 0 && extension[0] == '.';
    const auto requiredDestStrBufferLen = extensionStartIndex + static_cast<int32_t>(extensionHasDot ? extension.length() + 1 : extension.length() + 2);
    if (requiredDestStrBufferLen > destStrBufferLen)
    {
        return {};
    }

    memcpy(destStr, path.data(), extensionStartIndex);
    
    auto strLen = 0;
    if (extensionHasDot)
    {
        memcpy(&destStr[extensionStartIndex], extension.data(), extension.size());
        strLen = static_cast<int32_t>(extensionStartIndex + extension.size());
    }
    else
    {
        memcpy(&destStr[extensionStartIndex + 1], extension.data(), extension.size());
        destStr[extensionStartIndex] = '.';
        strLen = static_cast<int32_t>(extensionStartIndex + 1 + extension.size());
    }
    
    destStr[strLen] = '\0';
    return strLen;
}

std::u8string Path::GetFullPath(const std::u8string_view& path)
{
    if (path.length() <= 0)
    {
        return {};
    }
    
    std::u8string collapsedString;
    if (!IsPathRooted(path))
    {
        const auto collapsedStringLen = Combine(Directory::GetCurrentDirectory(), path, g_tempUtf8StrBuffer.data(), static_cast<int32_t>(g_tempUtf8StrBuffer.size()));
        if (collapsedStringLen.has_value() == false)
        {
            return {};
        }

        collapsedString = RemoveRelativeSegments(std::u8string_view(g_tempUtf8StrBuffer.data(), *collapsedStringLen));
    }
    else
    {
        collapsedString = RemoveRelativeSegments(path);
    }
    
    return collapsedString.length() == 0 ? u8"/" : collapsedString;
}

std::u8string Path::GetFullPath(const std::u8string_view& path, const std::u8string_view& basePath)
{
    return GetFullPath(Combine(basePath, path));
}

std::optional<int32_t> Path::GetRandomFileName(char8_t* destStr, int32_t destStrBufferLen)
{
    constexpr auto requiredDestStrBufferLen = 13;
    if (destStrBufferLen < requiredDestStrBufferLen)
    {
        return {};
    }
    
    auto r = Random();
    const auto strLen = requiredDestStrBufferLen - 1;
    for (auto i = 0; i < strLen; ++i)
    {
        destStr[i] = static_cast<char8_t>(r.Next(0, 1) == 0 ? r.Next('a', 'z') : r.Next('0', '9'));
    }
    
    destStr[8] = '.';
    destStr[strLen] = '\0';

    return strLen;
}

std::u8string Path::GetRandomFileName()
{
    const auto strLen = GetRandomFileName(g_tempUtf8StrBuffer.data(), static_cast<int32_t>(g_tempUtf8StrBuffer.size()));
    return {g_tempUtf8StrBuffer.data(), static_cast<size_t>(*strLen)};
}

std::u8string Path::GetTempPath()
{
    const auto strLen = GetTempPath(g_tempUtf8StrBuffer.data(), static_cast<int32_t>(g_tempUtf8StrBuffer.size()));
    return {g_tempUtf8StrBuffer.data(), static_cast<size_t>(*strLen)};
}

std::optional<int32_t> Path::GetTempPath(char8_t* destStr, int32_t destStrBufferLen)
{
    const char8_t tempEnvVarName[] = u8"TMPDIR";

    auto tempEnvVarValue = Environment::GetEnvironmentVariable(tempEnvVarName);
    if (!tempEnvVarValue)
    {
        const char8_t defaultTempPath[] = u8"/tmp/";
        if (static_cast<int32_t>(std::extent_v<decltype(defaultTempPath)>) > destStrBufferLen)
        {
            return {};
        }
        
        memcpy(destStr, defaultTempPath, std::extent_v<decltype(defaultTempPath)>);
        return static_cast<int32_t>(std::extent_v<decltype(defaultTempPath)>) - 1;
    }
    
    if (IsDirectorySeparator((*tempEnvVarValue)[tempEnvVarValue->length() - 1]))
    {
        if (static_cast<int32_t>(tempEnvVarValue->length()) + 1 > destStrBufferLen)
        {
            return {};
        }
        
        memcpy(destStr, tempEnvVarValue->data(), tempEnvVarValue->size());
        destStr[tempEnvVarValue->size()] = '\0';
        
        return static_cast<int32_t>(tempEnvVarValue->size());
    }

    if (static_cast<int32_t>(tempEnvVarValue->length()) + 2 > destStrBufferLen)
    {
        return {};
    }
    
    memcpy(destStr, tempEnvVarValue->data(), tempEnvVarValue->size());
    destStr[tempEnvVarValue->size()] = DirectorySeparatorChar;
    destStr[tempEnvVarValue->size() + 1] = '\0';
    
    return static_cast<int32_t>(tempEnvVarValue->size());
}

std::u8string Path::RemoveRelativeSegments(const std::u8string_view& path)
{
    std::u8string ret;
    for (decltype(path.length()) i = 0; i < path.length(); ++i)
    {
        auto c = path[i];
        if (IsDirectorySeparator(c) && i + 1 < path.length())
        {
            // Ignore the '/'
            if (IsDirectorySeparator(path[i + 1]))
            {
                continue;
            }
            
            // Ignore the "./"
            if (i + 2 >= path.length() || (IsDirectorySeparator(path[i + 2]) && path[i + 1] == '.'))
            {
                ++i;
                continue;
            }
            
            if (i + 3 < path.length() && IsDirectorySeparator(path[i + 3]) && path[i + 2] == '.' && path[i + 1] == '.')
            {
                auto j = ret.length() == 0 ? 0 : static_cast<int32_t>(ret.length()) - 1;
                for (; j >= 0; --j)
                {
                    if (IsDirectorySeparator(ret[j]))
                    {
                        ret.resize(static_cast<size_t>(j));
                        break;
                    }
                }
                
                if (j == 0)
                {
                    ret.resize(0);
                }
                
                i += 2;
                continue;
            }
        }

        // Normalize the directory separator
        if (c == AltDirectorySeparatorChar)
        {
            c = DirectorySeparatorChar;
        }
        
        ret += c;
    }

    return ret;
}

}
