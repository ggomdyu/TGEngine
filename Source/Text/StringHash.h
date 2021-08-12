#pragma once

#include <string>

#include "Core/TypeTraits.h"

#include "Hash.h"
#include "FixedString.h"

namespace tg
{
namespace detail
{

template <typename>
constexpr bool IsBasicString = false;

template <typename _Char, typename _Traits, typename _Allocator>
constexpr bool IsBasicString<std::basic_string<_Char, _Traits, _Allocator>> = true;

template <typename>
constexpr bool IsBasicStringView = false;

template <typename _Char>
constexpr bool IsBasicStringView<std::basic_string_view<_Char>> = true;

}

template <typename>
class BasicStringHash;

template <typename>
constexpr bool IsBasicStringHash = std::false_type::value;

template <typename _Type>
constexpr bool IsBasicStringHash<BasicStringHash<_Type>> = std::true_type::value;

template <typename _String>
class BasicStringHash final
{
/**@section Type */
public:
    using ValueType = std::remove_cvref_t<decltype(_String()[0])>;
    using StringType = std::conditional_t<IsChar<RawType<_String>> && std::is_pointer_v<_String>, std::basic_string_view<ValueType>, _String>;

/**@section Constructor */
public:
    constexpr BasicStringHash() noexcept = default;
    constexpr BasicStringHash(const BasicStringHash& str) = default;
    constexpr BasicStringHash(BasicStringHash&& str) noexcept = default;
    template <typename _String2> requires IsBasicStringHash<_String2>
    constexpr BasicStringHash(const _String2& str) noexcept;
    template <typename _String2> requires(!IsBasicStringHash<_String2>)
    constexpr BasicStringHash(const _String2& str) noexcept;

/**@section Destructor */
public:
    ~BasicStringHash() = default;

/**@section Operator */
public:
    BasicStringHash& operator=(const BasicStringHash& rhs) = delete;
    BasicStringHash& operator=(BasicStringHash&& rhs) noexcept = default;
    template <typename _String2>
    BasicStringHash& operator=(const _String2& rhs) noexcept;
    constexpr ValueType& operator[](int32_t index);
    constexpr ValueType operator[](int32_t index) const;
    template <typename _String2>
    constexpr bool operator==(const BasicStringHash<_String2>& rhs) const noexcept;
    constexpr bool operator==(const ValueType* rhs) const noexcept;
    template <typename _String2>
    constexpr bool operator!=(const BasicStringHash<_String2>& rhs) const noexcept;
    constexpr bool operator!=(const ValueType* rhs) const noexcept;
    template <typename _String2>
    constexpr bool operator<(const BasicStringHash<_String2>& rhs) const noexcept;
    template <typename _String2>
    constexpr bool operator<=(const BasicStringHash<_String2>& rhs) const noexcept;
    template <typename _String2>
    constexpr bool operator>(const BasicStringHash<_String2>& rhs) const noexcept;
    template <typename _String2>
    constexpr bool operator>=(const BasicStringHash<_String2>& rhs) const noexcept;

/**@section Method */
public:
    [[nodiscard]] int32_t CompareTo(const std::basic_string_view<ValueType>& str) noexcept;
    [[nodiscard]] int32_t IndexOf(const std::basic_string_view<ValueType>& str, int32_t startIndex = 0) const;
    [[nodiscard]] int32_t IndexOf(ValueType c, int32_t startIndex = 0) const;
    template <typename _Predicate>
    [[nodiscard]] int32_t IndexOfAny(const _Predicate& predicate, int32_t startIndex = 0) const;
    [[nodiscard]] int32_t LastIndexOf(const std::basic_string_view<ValueType>& str) const;
    [[nodiscard]] int32_t LastIndexOf(const std::basic_string_view<ValueType>& str, int32_t startIndex) const;
    [[nodiscard]] int32_t LastIndexOf(ValueType c) const;
    [[nodiscard]] int32_t LastIndexOf(ValueType c, int32_t startIndex) const;
    template <typename _Predicate>
    [[nodiscard]] int32_t LastIndexOfAny(const _Predicate& predicate) const;
    template <typename _Predicate>
    [[nodiscard]] int32_t LastIndexOfAny(const _Predicate& predicate, int32_t startIndex) const;
    [[nodiscard]] constexpr const ValueType* Data() const noexcept;
    [[nodiscard]] constexpr int32_t Length() const noexcept;
    [[nodiscard]] constexpr size_t GetHashCode() const noexcept;

/**@section Variable */
public:
    StringType m_str{};
    size_t m_hashCode = 0;
};

template <typename _String>
template <typename _String2> requires IsBasicStringHash<_String2>
constexpr BasicStringHash<_String>::BasicStringHash(const _String2& str) noexcept :
    m_str(str.Data(), str.Length()),
    m_hashCode(str.GetHashCode())
{
}

template <typename _String>
template <typename _String2> requires(!IsBasicStringHash<_String2>)
constexpr BasicStringHash<_String>::BasicStringHash(const _String2& str) noexcept :
    m_str(str),
    m_hashCode(std::hash<BasicStringHash>{}(this->Data()))
{
}

template <typename _String>
template <typename _String2>
BasicStringHash<_String>& BasicStringHash<_String>::operator=(const _String2& rhs) noexcept
{
    if constexpr (IsBasicStringHash<_String2>)
    {
        m_str = rhs.m_str;
        m_hashCode = rhs.m_hashCode;
    }
    else
    {
        m_str = rhs;
        m_hashCode = X65599Hash(&rhs[0]);
    }

    return *this;
}

template <typename _String>
constexpr typename BasicStringHash<_String>::ValueType& BasicStringHash<_String>::operator[](int32_t index)
{
    return m_str[index];
}

template <typename _String>
constexpr typename BasicStringHash<_String>::ValueType BasicStringHash<_String>::operator[](int32_t index) const
{
    return m_str[index];
}

template <typename _String>
template <typename _String2>
constexpr bool BasicStringHash<_String>::operator==(const BasicStringHash<_String2>& rhs) const noexcept
{
    return m_hashCode == rhs.GetHashCode() && m_str == rhs.m_str;
}

template <typename _String>
constexpr bool BasicStringHash<_String>::operator==(const ValueType* rhs) const noexcept
{
    return m_hashCode == std::hash<BasicStringHash>{}(rhs) && BasicStringTraits<ValueType>::Compare(this->Data(), this->Length(), rhs, BasicStringTraits<ValueType>::Length(rhs)) == 0;
}

template <typename _String>
template <typename _String2>
constexpr bool BasicStringHash<_String>::operator!=(const BasicStringHash<_String2>& rhs) const noexcept
{
    return !this->operator==(rhs);
}

template <typename _String>
constexpr bool BasicStringHash<_String>::operator!=(const ValueType* rhs) const noexcept
{
    return !this->operator!=(rhs);
}

template <typename _String>
template <typename _String2>
constexpr bool BasicStringHash<_String>::operator<(const BasicStringHash<_String2>& rhs) const noexcept
{
    return m_hashCode < rhs.GetHashCode();
}

template <typename _String>
template <typename _String2>
constexpr bool BasicStringHash<_String>::operator<=(const BasicStringHash<_String2>& rhs) const noexcept
{
    return m_hashCode <= rhs.GetHashCode();
}

template <typename _String>
template <typename _String2>
constexpr bool BasicStringHash<_String>::operator>(const BasicStringHash<_String2>& rhs) const noexcept
{
    return m_hashCode > rhs.GetHashCode();
}

template <typename _String>
template <typename _String2>
constexpr bool BasicStringHash<_String>::operator>=(const BasicStringHash<_String2>& rhs) const noexcept
{
    return m_hashCode >= rhs.GetHashCode();
}

template <typename _String>
constexpr size_t BasicStringHash<_String>::GetHashCode() const noexcept
{
    return m_hashCode;
}

template <typename _String>
int32_t BasicStringHash<_String>::IndexOf(ValueType c, int32_t startIndex) const
{
    ValueType str[] = {c, 0};
    return BasicStringTraits<ValueType>::IndexOf(this->Data() + startIndex, this->Length() - startIndex, str, 1) + startIndex;

}

template <typename _String>
int32_t BasicStringHash<_String>::CompareTo(const std::basic_string_view<ValueType>& str) noexcept
{
    return BasicStringTraits<ValueType>::Compare(this->Data(), this->Length(), str.data(), str.length());
}

template <typename _String>
int32_t BasicStringHash<_String>::IndexOf(const std::basic_string_view<ValueType>& str, int32_t startIndex) const
{
    return BasicStringTraits<ValueType>::IndexOf(this->Data() + startIndex, this->Length() - startIndex, str.data(), str.length()) + startIndex;
}

template <typename _String>
template <typename _Predicate>
int32_t BasicStringHash<_String>::IndexOfAny(const _Predicate& predicate, int32_t startIndex) const
{
    return BasicStringTraits<ValueType>::IndexOfAny(this->Data() + startIndex, this->Length() - startIndex, predicate) + startIndex;
}

template <typename _String>
int32_t BasicStringHash<_String>::LastIndexOf(ValueType c, int32_t startIndex) const
{
    ValueType str[] = {c, ValueType(0)};
    return BasicStringTraits<ValueType>::LastIndexOf(this->Data(), startIndex + 1, str, 1);
}

template <typename _String>
int32_t BasicStringHash<_String>::LastIndexOf(const std::basic_string_view<ValueType>& str) const
{
    return BasicStringTraits<ValueType>::LastIndexOf(this->Data(), this->Length(), str, str.length());
}

template <typename _String>
int32_t BasicStringHash<_String>::LastIndexOf(const std::basic_string_view<ValueType>& str, int32_t startIndex) const
{
    return BasicStringTraits<ValueType>::LastIndexOf(this->Data(), startIndex, str, str.length());
}

template <typename _String>
int32_t BasicStringHash<_String>::LastIndexOf(ValueType c) const
{
    return BasicStringTraits<ValueType>::LastIndexOf(this->Data(), this->Length(), &c, 1);
}

template <typename _String>
template <typename _Predicate>
int32_t BasicStringHash<_String>::LastIndexOfAny(const _Predicate& predicate) const
{
    return BasicStringTraits<ValueType>::LastIndexOfAny(this->Data(), this->Length(), predicate);
}

template <typename _String>
template <typename _Predicate>
int32_t BasicStringHash<_String>::LastIndexOfAny(const _Predicate& predicate, int32_t startIndex) const
{
    return BasicStringTraits<ValueType>::LastIndexOfAny(this->Data(), startIndex, predicate);
}

template <typename _String>
constexpr const typename BasicStringHash<_String>::ValueType* BasicStringHash<_String>::Data() const noexcept
{
    if constexpr (detail::IsBasicString<StringType> || detail::IsBasicStringView<StringType>)
    {
        return m_str.data();
    }
    else
    {
        return m_str.Data();
    }
}

template <typename _String>
constexpr int32_t BasicStringHash<_String>::Length() const noexcept
{
    if constexpr (detail::IsBasicString<StringType> || detail::IsBasicStringView<StringType>)
    {
        return static_cast<int32_t>(m_str.length());
    }
    else
    {
        return static_cast<int32_t>(m_str.Length());
    }
}

using StringHash = BasicStringHash<std::string>;
using StringViewHash = BasicStringHash<std::string_view>;
using FixedString8Hash = BasicStringHash<FixedString8>;
using FixedString16Hash = BasicStringHash<FixedString16>;
using FixedString32Hash = BasicStringHash<FixedString32>;
using FixedString64Hash = BasicStringHash<FixedString64>;
using FixedString128Hash = BasicStringHash<FixedString128>;
using FixedString256Hash = BasicStringHash<FixedString256>;
using FixedString512Hash = BasicStringHash<FixedString512>;
using FixedString1024Hash = BasicStringHash<FixedString1024>;
using FixedString2048Hash = BasicStringHash<FixedString2048>;
using FixedString4096Hash = BasicStringHash<FixedString4096>;
using FixedString8192Hash = BasicStringHash<FixedString8192>;
    
using U8StringHash = BasicStringHash<std::u8string>;
using U8StringViewHash = BasicStringHash<std::u8string_view>;
using U8FixedString8Hash = BasicStringHash<U8FixedString8>;
using U8FixedString16Hash = BasicStringHash<U8FixedString16>;
using U8FixedString32Hash = BasicStringHash<U8FixedString32>;
using U8FixedString64Hash = BasicStringHash<U8FixedString64>;
using U8FixedString128Hash = BasicStringHash<U8FixedString128>;
using U8FixedString256Hash = BasicStringHash<U8FixedString256>;
using U8FixedString512Hash = BasicStringHash<U8FixedString512>;
using U8FixedString1024Hash = BasicStringHash<U8FixedString1024>;
using U8FixedString2048Hash = BasicStringHash<U8FixedString2048>;
using U8FixedString4096Hash = BasicStringHash<U8FixedString4096>;
using U8FixedString8192Hash = BasicStringHash<U8FixedString8192>;

using U16StringHash = BasicStringHash<std::u16string>;
using U16StringViewHash = BasicStringHash<std::u16string_view>;
using U16FixedString8Hash = BasicStringHash<U16FixedString8>;
using U16FixedString16Hash = BasicStringHash<U16FixedString16>;
using U16FixedString32Hash = BasicStringHash<U16FixedString32>;
using U16FixedString64Hash = BasicStringHash<U16FixedString64>;
using U16FixedString128Hash = BasicStringHash<U16FixedString128>;
using U16FixedString256Hash = BasicStringHash<U16FixedString256>;
using U16FixedString512Hash = BasicStringHash<U16FixedString512>;
using U16FixedString1024Hash = BasicStringHash<U16FixedString1024>;
using U16FixedString2048Hash = BasicStringHash<U16FixedString2048>;
using U16FixedString4096Hash = BasicStringHash<U16FixedString4096>;
using U16FixedString8192Hash = BasicStringHash<U16FixedString8192>;

using U32StringHash = BasicStringHash<std::u32string>;
using U32StringViewHash = BasicStringHash<std::u32string_view>;
using U32FixedString8Hash = BasicStringHash<U32FixedString8>;
using U32FixedString16Hash = BasicStringHash<U32FixedString16>;
using U32FixedString32Hash = BasicStringHash<U32FixedString32>;
using U32FixedString64Hash = BasicStringHash<U32FixedString64>;
using U32FixedString128Hash = BasicStringHash<U32FixedString128>;
using U32FixedString256Hash = BasicStringHash<U32FixedString256>;
using U32FixedString512Hash = BasicStringHash<U32FixedString512>;
using U32FixedString1024Hash = BasicStringHash<U32FixedString1024>;
using U32FixedString2048Hash = BasicStringHash<U32FixedString2048>;
using U32FixedString4096Hash = BasicStringHash<U32FixedString4096>;
using U32FixedString8192Hash = BasicStringHash<U32FixedString8192>;

}

namespace std
{

template <typename _String>
struct hash<tg::BasicStringHash<_String>>
{
/* @section Method */
public:
    auto operator()(const tg::BasicStringHash<_String>& rhs) const noexcept
    {
        return rhs.GetHashCode();
    }
    
    auto operator()(const char* rhs) const noexcept
    {
        return tg::X65599Hash(rhs);
    }
};

}
