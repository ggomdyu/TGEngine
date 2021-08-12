#pragma once

#include <array>
#include <type_traits>

#include "StringTraits.h"

namespace tg
{

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits = BasicStringTraits<_Char>>
class BasicFixedString final
{
    static_assert(std::is_same<_Char, typename _StringTraits::CharType>::value, "_StringTraits's character type doesn't match with _Char.");

/**@section Type */
public:
    using CharType = _StringTraits;

/**@section Constructor */
public:
    constexpr BasicFixedString() noexcept = default;
    BasicFixedString(const _Char* str, int32_t strLen);
    BasicFixedString(const _Char* str);
    BasicFixedString(const std::basic_string_view<_Char>& str);
    BasicFixedString(_Char c, int32_t chCount);
    template <int32_t _CharBufferSize2>
    BasicFixedString(const BasicFixedString<_Char, _CharBufferSize2, _StringTraits>& str);

/**@section Operator */
public:
    BasicFixedString operator+(const std::basic_string_view<_Char>& str) const;
    template <int32_t _CharBufferSize2>
    BasicFixedString operator+(const BasicFixedString<_Char, _CharBufferSize2, _StringTraits>& str) const;
    BasicFixedString operator+(_Char c) const;
    BasicFixedString& operator+=(const std::basic_string_view<_Char>& str);
    template <int32_t _CharBufferSize2>
    BasicFixedString& operator+=(const BasicFixedString<_Char, _CharBufferSize2, _StringTraits>& str);
    BasicFixedString& operator+=(_Char c);
    bool operator!=(const std::basic_string_view<_Char>& str) const;
    template <int32_t _CharBufferSize2>
    bool operator!=(const BasicFixedString<_Char, _CharBufferSize2, _StringTraits>& str) const;
    bool operator==(const std::basic_string_view<_Char>& str) const;
    template <int32_t _CharBufferSize2>
    bool operator==(const BasicFixedString<_Char, _CharBufferSize2, _StringTraits>& str) const;
    _Char operator[](int32_t index) const;
    _Char& operator[](int32_t index);

/**@section Method */
public:
    template <int32_t _CharBufferSize2>
    [[nodiscard]] BasicFixedString<_Char, _CharBufferSize + _CharBufferSize2, _StringTraits> Expand(const BasicFixedString<_Char, _CharBufferSize2, _StringTraits>& str) const;
    template <int32_t _CharBufferSize2>
    [[nodiscard]] int32_t CompareTo(const BasicFixedString<_Char, _CharBufferSize2, _StringTraits>& str) const;
    [[nodiscard]] int32_t CompareTo(const std::basic_string_view<_Char>& str) const;
    [[nodiscard]] int32_t IndexOf(const std::basic_string_view<_Char>& str, int32_t startIndex = 0) const;
    [[nodiscard]] int32_t IndexOf(_Char c, int32_t startIndex = 0) const;
    template <typename _Predicate>
    [[nodiscard]] int32_t IndexOfAny(const _Predicate& predicate, int32_t startIndex = 0) const;
    [[nodiscard]] int32_t LastIndexOf(const std::basic_string_view<_Char>& str) const;
    [[nodiscard]] int32_t LastIndexOf(const std::basic_string_view<_Char>& str, int32_t startIndex) const;
    [[nodiscard]] int32_t LastIndexOf(_Char c) const;
    [[nodiscard]] int32_t LastIndexOf(_Char c, int32_t startIndex) const;
    template <typename _Predicate>
    [[nodiscard]] int32_t LastIndexOfAny(const _Predicate& predicate) const;
    template <typename _Predicate>
    [[nodiscard]] int32_t LastIndexOfAny(const _Predicate& predicate, int32_t startIndex) const;
    [[nodiscard]] const _Char* Data() const noexcept;
    [[nodiscard]] int32_t Length() const noexcept;
    [[nodiscard]] static constexpr int32_t Capacity() noexcept;

/**@section Variable */
protected:
    std::array<_Char, _CharBufferSize> m_str = {};
    int32_t m_strLen = 0;
};

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits = BasicStringTraits<_Char>>
BasicFixedString(const _Char(&)[_CharBufferSize]) -> BasicFixedString<_Char, _CharBufferSize, _StringTraits>;

template <typename>
constexpr bool IsBasicFixedString = std::false_type::value;

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
constexpr bool IsBasicFixedString<BasicFixedString<_Char, _CharBufferSize, _StringTraits>> = std::true_type::value;

using FixedString8 = BasicFixedString<char, 8>;
using FixedString16 = BasicFixedString<char, 16>;
using FixedString32 = BasicFixedString<char, 32>;
using FixedString64 = BasicFixedString<char, 64>;
using FixedString128 = BasicFixedString<char, 128>;
using FixedString256 = BasicFixedString<char, 256>;
using FixedString512 = BasicFixedString<char, 512>;
using FixedString1024 = BasicFixedString<char, 1024>;
using FixedString2048 = BasicFixedString<char, 2048>;
using FixedString4096 = BasicFixedString<char, 4096>;
using FixedString8192 = BasicFixedString<char, 8192>;

using U8FixedString8 = BasicFixedString<char8_t, 8>;
using U8FixedString16 = BasicFixedString<char8_t, 16>;
using U8FixedString32 = BasicFixedString<char8_t, 32>;
using U8FixedString64 = BasicFixedString<char8_t, 64>;
using U8FixedString128 = BasicFixedString<char8_t, 128>;
using U8FixedString256 = BasicFixedString<char8_t, 256>;
using U8FixedString512 = BasicFixedString<char8_t, 512>;
using U8FixedString1024 = BasicFixedString<char8_t, 1024>;
using U8FixedString2048 = BasicFixedString<char8_t, 2048>;
using U8FixedString4096 = BasicFixedString<char8_t, 4096>;
using U8FixedString8192 = BasicFixedString<char8_t, 8192>;

using U16FixedString8 = BasicFixedString<char16_t, 8>;
using U16FixedString16 = BasicFixedString<char16_t, 16>;
using U16FixedString32 = BasicFixedString<char16_t, 32>;
using U16FixedString64 = BasicFixedString<char16_t, 64>;
using U16FixedString128 = BasicFixedString<char16_t, 128>;
using U16FixedString256 = BasicFixedString<char16_t, 256>;
using U16FixedString512 = BasicFixedString<char16_t, 512>;
using U16FixedString1024 = BasicFixedString<char16_t, 1024>;
using U16FixedString2048 = BasicFixedString<char16_t, 2048>;
using U16FixedString4096 = BasicFixedString<char16_t, 4096>;
using U16FixedString8192 = BasicFixedString<char16_t, 8192>;

using U32FixedString8 = BasicFixedString<char32_t, 8>;
using U32FixedString16 = BasicFixedString<char32_t, 16>;
using U32FixedString32 = BasicFixedString<char32_t, 32>;
using U32FixedString64 = BasicFixedString<char32_t, 64>;
using U32FixedString128 = BasicFixedString<char32_t, 128>;
using U32FixedString256 = BasicFixedString<char32_t, 256>;
using U32FixedString512 = BasicFixedString<char32_t, 512>;
using U32FixedString1024 = BasicFixedString<char32_t, 1024>;
using U32FixedString2048 = BasicFixedString<char32_t, 2048>;
using U32FixedString4096 = BasicFixedString<char32_t, 4096>;
using U32FixedString8192 = BasicFixedString<char32_t, 8192>;

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
BasicFixedString<_Char, _CharBufferSize, _StringTraits>::BasicFixedString(const _Char* str, int32_t strLen) :
    m_strLen(strLen)
{
    _StringTraits::Append(str, strLen, m_str.data(), 0, static_cast<int32_t>(m_str.size()));
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
BasicFixedString<_Char, _CharBufferSize, _StringTraits>::BasicFixedString(const _Char* str) :
    BasicFixedString(std::basic_string_view<_Char>(str))
{
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
BasicFixedString<_Char, _CharBufferSize, _StringTraits>::BasicFixedString(const std::basic_string_view<_Char>& str) :
    BasicFixedString(str.data(), static_cast<int32_t>(str.length()))
{
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
BasicFixedString<_Char, _CharBufferSize, _StringTraits>::BasicFixedString(_Char c, int32_t chCount) :
    m_strLen(chCount)
{
    _StringTraits::Append(c, chCount, m_str.data(), 0, _CharBufferSize);
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
template <int32_t _CharBufferSize2>
BasicFixedString<_Char, _CharBufferSize, _StringTraits>::BasicFixedString(const BasicFixedString<_Char, _CharBufferSize2, _StringTraits>& str) :
    BasicFixedString(std::basic_string_view<_Char>(str.Data(), str.Length()))
{
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
template <int32_t _CharBufferSize2>
BasicFixedString<_Char, _CharBufferSize, _StringTraits> BasicFixedString<_Char, _CharBufferSize, _StringTraits>::operator+(const BasicFixedString<_Char, _CharBufferSize2, _StringTraits>& str) const
{
    return BasicFixedString<_Char, _CharBufferSize, _StringTraits>(*this) += str;
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
BasicFixedString<_Char, _CharBufferSize, _StringTraits> BasicFixedString<_Char, _CharBufferSize, _StringTraits>::operator+(const std::basic_string_view<_Char>& str) const
{
    return BasicFixedString<_Char, _CharBufferSize, _StringTraits>(*this) += str;
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
BasicFixedString<_Char, _CharBufferSize, _StringTraits> BasicFixedString<_Char, _CharBufferSize, _StringTraits>::operator+(_Char c) const
{
    return BasicFixedString<_Char, _CharBufferSize, _StringTraits>(*this) += c;
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
template <int32_t _CharBufferSize2>
BasicFixedString<_Char, _CharBufferSize, _StringTraits>& BasicFixedString<_Char, _CharBufferSize, _StringTraits>::operator+=(const BasicFixedString<_Char, _CharBufferSize2, _StringTraits>& str)
{
    return this->operator+=(std::basic_string_view<_Char>(str.Data(), static_cast<size_t>(str.Length())));
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
BasicFixedString<_Char, _CharBufferSize, _StringTraits>& BasicFixedString<_Char, _CharBufferSize, _StringTraits>::operator+=(const std::basic_string_view<_Char>& str)
{
    _StringTraits::Append(str.data(), static_cast<int32_t>(str.length()), m_str.data(), m_strLen, static_cast<int32_t>(m_str.size()));
    m_strLen += static_cast<int32_t>(str.length());

    return *this;
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
BasicFixedString<_Char, _CharBufferSize, _StringTraits>& BasicFixedString<_Char, _CharBufferSize, _StringTraits>::operator+=(_Char c)
{
    _StringTraits::Append(c, 1, m_str.data(), m_strLen, _CharBufferSize);
    m_strLen += 1;

    return *this;
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
bool BasicFixedString<_Char, _CharBufferSize, _StringTraits>::operator!=(const std::basic_string_view<_Char>& str) const
{
    return !this->operator==(str);
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
template <int32_t _CharBufferSize2>
bool BasicFixedString<_Char, _CharBufferSize, _StringTraits>::operator!=(const BasicFixedString<_Char, _CharBufferSize2, _StringTraits>& str) const
{
    return !this->operator==(str);
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
template <int32_t _CharBufferSize2>
bool BasicFixedString<_Char, _CharBufferSize, _StringTraits>::operator==(const BasicFixedString<_Char, _CharBufferSize2, _StringTraits>& str) const
{
    return operator==(std::basic_string_view<_Char>(str.Data(), static_cast<size_t>(str.Length())));
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
bool BasicFixedString<_Char, _CharBufferSize, _StringTraits>::operator==(const std::basic_string_view<_Char>& str) const
{
    return _StringTraits::Compare(m_str.data(), m_strLen, str.data(), static_cast<int32_t>(str.length())) == 0;
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
_Char BasicFixedString<_Char, _CharBufferSize, _StringTraits>::operator[](int32_t index) const
{
    return m_str[index];
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
_Char& BasicFixedString<_Char, _CharBufferSize, _StringTraits>::operator[](int32_t index)
{
    return m_str[index];
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
template <int32_t _CharBufferSize2>
BasicFixedString<_Char, _CharBufferSize + _CharBufferSize2, _StringTraits> BasicFixedString<_Char, _CharBufferSize, _StringTraits>::Expand(const BasicFixedString<_Char, _CharBufferSize2, _StringTraits>& str) const
{
    return BasicFixedString<_Char, _CharBufferSize + _CharBufferSize2, _StringTraits>(*this) += str;
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
template <int32_t _CharBufferSize2>
int32_t BasicFixedString<_Char, _CharBufferSize, _StringTraits>::CompareTo(const BasicFixedString<_Char, _CharBufferSize2, _StringTraits>& str) const
{
    return CompareTo(std::basic_string_view<_Char>(str.Data(), str.Length));
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
int32_t BasicFixedString<_Char, _CharBufferSize, _StringTraits>::CompareTo(const std::basic_string_view<_Char>& str) const
{
    return _StringTraits::Compare(m_str.data(), m_strLen, str.data(), str.length());
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
template <typename _Predicate>
int32_t BasicFixedString<_Char, _CharBufferSize, _StringTraits>::IndexOfAny(const _Predicate& predicate, int32_t startIndex) const
{
    return _StringTraits::IndexOfAny(m_str.data() + startIndex, m_strLen - startIndex, predicate) + startIndex;
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
int32_t BasicFixedString<_Char, _CharBufferSize, _StringTraits>::IndexOf(_Char c, int32_t startIndex) const
{
    _Char str[] = {c, 0};
    return _StringTraits::IndexOf(m_str.data() + startIndex, m_strLen - startIndex, str, 1) + startIndex;
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
int32_t BasicFixedString<_Char, _CharBufferSize, _StringTraits>::IndexOf(const std::basic_string_view<_Char>& str, int32_t startIndex) const
{
    return _StringTraits::IndexOf(m_str.data() + startIndex, m_strLen - startIndex, str, str.length()) + startIndex;
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
int32_t BasicFixedString<_Char, _CharBufferSize, _StringTraits>::LastIndexOf(_Char c, int32_t startIndex) const
{
    _Char str[] = {c, 0};
    return _StringTraits::LastIndexOf(m_str.data(), startIndex + 1, str, 1);
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
int32_t BasicFixedString<_Char, _CharBufferSize, _StringTraits>::LastIndexOf(const std::basic_string_view<_Char>& str) const
{
    return _StringTraits::LastIndexOf(m_str.data(), m_strLen, str, str.length());
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
int32_t BasicFixedString<_Char, _CharBufferSize, _StringTraits>::LastIndexOf(const std::basic_string_view<_Char>& str, int32_t startIndex) const
{
    return _StringTraits::LastIndexOf(m_str.data(), startIndex, str, str.length());
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
int32_t BasicFixedString<_Char, _CharBufferSize, _StringTraits>::LastIndexOf(_Char c) const
{
    return _StringTraits::LastIndexOf(m_str.data(), m_strLen, &c, 1);
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
template <typename _Predicate>
int32_t BasicFixedString<_Char, _CharBufferSize, _StringTraits>::LastIndexOfAny(const _Predicate& predicate) const
{
    return _StringTraits::LastIndexOfAny(m_str.data(), m_strLen, predicate);
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
template <typename _Predicate>
int32_t BasicFixedString<_Char, _CharBufferSize, _StringTraits>::LastIndexOfAny(const _Predicate& predicate, int32_t startIndex) const
{
    return _StringTraits::LastIndexOfAny(m_str.data(), startIndex, predicate);
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
const _Char* BasicFixedString<_Char, _CharBufferSize, _StringTraits>::Data() const noexcept
{
    return m_str.data();
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
int32_t BasicFixedString<_Char, _CharBufferSize, _StringTraits>::Length() const noexcept
{
    return m_strLen;
}

template <typename _Char, int32_t _CharBufferSize, typename _StringTraits>
constexpr int32_t BasicFixedString<_Char, _CharBufferSize, _StringTraits>::Capacity() noexcept
{
    return _CharBufferSize;
}

}
