#pragma once

#include <cstddef>

#include "Core/TypeTraits.h"

namespace tg
{

template <typename _Char> requires IsChar<_Char>
constexpr int32_t X65599Hash(_Char c) noexcept
{
    return c ^ (c >> 16);
}
    
template <typename _Char> requires IsChar<_Char>
constexpr int32_t X65599Hash(const _Char* str) noexcept
{
    if (str == nullptr)
    {
        return 0;
    }
    
    int32_t hashValue = 0;
    for (int32_t i = 0; str[i] != 0; ++i)
    {
        hashValue = 65599 * hashValue + str[i];
    }

    return hashValue ^ (hashValue >> 16);
}

}
