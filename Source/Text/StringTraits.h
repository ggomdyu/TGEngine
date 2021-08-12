#pragma once

#include <algorithm>
#include <cassert>
#include <cctype>
#include <cstdarg>
#include <memory>

namespace tg
{

template <typename _Char>
class BasicStringTraits final
{
/**@section Type */
public:
    using CharType = _Char;

/**@section Constructor */
public:
    BasicStringTraits() = delete;

/**@section Method */
public:
    static void Append(const _Char* srcStr, int32_t srcStrLen, _Char* destStr, int32_t destStrLen, int32_t destStrBufferLen);
    template <size_t _DestStrBufferLen>
    static void Append(const _Char* srcStr, int32_t srcStrLen, _Char(&destStr)[_DestStrBufferLen], int32_t destStrLen);
    static void Append(_Char c, int32_t count, _Char* destStr, int32_t destStrLen, int32_t destStrBufferLen);
    template <size_t _DestStrBufferLen>
    static void Append(_Char c, int32_t count, _Char(&destStr)[_DestStrBufferLen], int32_t destStrLen);
    [[nodiscard]] static int32_t IndexOf(const _Char* str, int32_t strLen, const _Char* subStr, int32_t subStrLen);
    template <typename _Predicate>
    [[nodiscard]] static int32_t IndexOfAny(const _Char* str, int32_t strLen, const _Predicate& predicate);
    [[nodiscard]] static int32_t LastIndexOf(const _Char* str, int32_t strLen, const _Char* subStr, int32_t subStrLen);
    template <typename _Predicate>
    [[nodiscard]] static constexpr int32_t LastIndexOfAny(const _Char* str, int32_t strLen, const _Predicate& predicate);
    [[nodiscard]] static constexpr int32_t Compare(const _Char* lhsStr, int32_t lhsStrLen, const _Char* rhsStr, int32_t rhsStrLen);
    [[nodiscard]] static constexpr int32_t Length(const _Char* str) noexcept;
    static void Swap(_Char* srcStr, int32_t srcStrLen, _Char* destStr, int32_t destStrLen);
    template <size_t _DestStrBufferLen>
    static void ToLower(const _Char* srcStr, int32_t srcStrLen, _Char(&destStr)[_DestStrBufferLen]);
    template <size_t _DestStrBufferLen>
    static void ToUpper(const _Char* srcStr, int32_t srcStrLen, _Char(&destStr)[_DestStrBufferLen]);
};

using StringTraits = BasicStringTraits<char>;
using U16StringTraits = BasicStringTraits<char16_t>;
using U32StringTraits = BasicStringTraits<char32_t>;
using WStringTraits = BasicStringTraits<wchar_t>;

template <typename _Char>
template <std::size_t _DestStrBufferLen>
void BasicStringTraits<_Char>::Append(const _Char* srcStr, int32_t srcStrLen, _Char(&destStr)[_DestStrBufferLen], int32_t destStrLen)
{
    Append(srcStr, srcStrLen, destStr, destStrLen, _DestStrBufferLen);
}

template <typename _Char>
void BasicStringTraits<_Char>::Append(const _Char* srcStr, int32_t srcStrLen, _Char* destStr, int32_t destStrLen, int32_t destStrBufferLen)
{
    assert(destStrBufferLen > srcStrLen + destStrLen);

    memcpy(&destStr[destStrLen], srcStr, sizeof(_Char) * srcStrLen);
    destStr[srcStrLen + destStrLen] = {};
}

template <typename _Char>
template <std::size_t _DestStrBufferLen>
void BasicStringTraits<_Char>::Append(_Char c, int32_t count, _Char(&destStr)[_DestStrBufferLen], int32_t destStrLen)
{
    Append(destStr, destStrLen, _DestStrBufferLen, c, count);
}

template <typename _Char>
void BasicStringTraits<_Char>::Append(_Char c, int32_t count, _Char* destStr, int32_t destStrLen, int32_t destStrBufferLen)
{
    assert(destStrBufferLen > count + destStrLen);

    count += destStrLen;
    while (destStrLen < count)
    {
        destStr[destStrLen++] = c;
    }

    destStr[destStrLen] = {};
}

template <typename _Char>
int32_t BasicStringTraits<_Char>::IndexOf(const _Char* str, int32_t strLen, const _Char* subStr, int32_t subStrLen)
{
    const _Char* foundStr = std::search(str, str + strLen, subStr, subStr + subStrLen);
    if (foundStr != str + strLen)
    {
        return static_cast<int32_t>(foundStr - str);
    }

    return -1;
}

template <typename _Char>
template <typename _Predicate>
int32_t BasicStringTraits<_Char>::IndexOfAny(const _Char* str, int32_t strLen, const _Predicate& predicate)
{
    const _Char* foundStr = std::find_if(str, str + strLen, predicate);
    if (foundStr != str + strLen)
    {
        return static_cast<int32_t>(foundStr - str);
    }

    return -1;
}

template <typename _Char>
int32_t BasicStringTraits<_Char>::LastIndexOf(const _Char* str, int32_t strLen, const _Char* subStr, int32_t subStrLen)
{
    const _Char* foundStr = std::find_end(str, str + strLen, subStr, subStr + subStrLen);
    if (foundStr != str + strLen)
    {
        return static_cast<int32_t>(foundStr - str);
    }

    return -1;
}

template <typename _Char>
template <typename _Predicate>
constexpr int32_t BasicStringTraits<_Char>::LastIndexOfAny(const _Char* str, int32_t strLen, const _Predicate& predicate)
{
    for (auto i = strLen - 1; i >= 0; --i)
    {
        if (predicate(str[i]))
        {
            return i;
        }
    }

    return -1;
}

template <typename _Char>
constexpr int32_t BasicStringTraits<_Char>::Compare(const _Char* lhsStr, int32_t lhsStrLen, const _Char* rhsStr, int32_t rhsStrLen)
{
    auto ans = std::char_traits<_Char>::compare(lhsStr, rhsStr, std::min(lhsStrLen, rhsStrLen));
    if (ans != 0)
    {
        return ans;
    }

    if (lhsStrLen < rhsStrLen)
    {
        return -1;
    }
    if (lhsStrLen > rhsStrLen)
    {
        return 1;
    }

    return 0;
}

template <typename _Char>
constexpr int32_t BasicStringTraits<_Char>::Length(const _Char* str) noexcept
{
    return static_cast<int32_t>(std::char_traits<_Char>::length(str));
}

template <typename _Char>
void BasicStringTraits<_Char>::Swap(_Char* srcStr, int32_t srcStrLen, _Char* destStr, int32_t destStrLen)
{
    std::swap_ranges(srcStr, srcStr + srcStrLen, destStr);
}

template <typename _Char>
template <std::size_t _DestStrBufferLen>
void BasicStringTraits<_Char>::ToLower(const _Char* srcStr, int32_t srcStrLen, _Char(&destStr)[_DestStrBufferLen])
{
    assert(_DestStrBufferLen > srcStrLen);

    std::transform(srcStr, srcStr + srcStrLen, destStr, ::tolower);
}

template <typename _Char>
template <std::size_t _DestStrBufferLen>
void BasicStringTraits<_Char>::ToUpper(const _Char* srcStr, int32_t srcStrLen, _Char(&destStr)[_DestStrBufferLen])
{
    assert(_DestStrBufferLen > srcStrLen);

    std::transform(srcStr, srcStr + srcStrLen, destStr, ::toupper);
}

}
