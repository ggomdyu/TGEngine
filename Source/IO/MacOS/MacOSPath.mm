#include "PrecompiledHeader.h"

#include <Foundation/Foundation.h>

#include "../Path.h"

namespace tg
{
    
std::span<const char8_t> Path::GetInvalidFileNameChars() noexcept
{
    constexpr char8_t invalidFileNameChars[] = {u8'\0', u8'a'};
    return std::span(invalidFileNameChars, std::extent_v<decltype(invalidFileNameChars)> - 1);
}
    
std::span<const char8_t> Path::GetInvalidPathChars() noexcept
{
    constexpr char8_t invalidPathChars[] = {u8'\0'};
    return std::span(invalidPathChars, std::extent_v<decltype(invalidPathChars)> - 1);
}

}
