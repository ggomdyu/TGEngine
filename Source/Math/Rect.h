#pragma once

#include <cstdint>
#include <fmt/format.h>
#include <span>
#include <string>
#include <type_traits>

#include "Core/Concepts.h"

namespace tg
{

template <typename _Value> requires Arithmetic<_Value>
struct BasicRect
{
/**@section Type */
public:
    using ValueType = _Value;

/**@section Constructor */
public:
    constexpr BasicRect() noexcept = default;
    constexpr BasicRect(const _Value& x, const _Value& y, const _Value& width, const _Value& height) noexcept;
    template <typename _Value2> requires Arithmetic<_Value>
    constexpr BasicRect(const BasicRect<_Value2>& rhs) noexcept;

/**@section Operator */
public:
    constexpr BasicRect operator-() const noexcept;
    template <typename _Value2> requires Arithmetic<_Value>
    BasicRect& operator=(const BasicRect<_Value2>& rhs) noexcept;
    constexpr bool operator==(const BasicRect& rhs) const noexcept;
    constexpr bool operator!=(const BasicRect& rhs) const noexcept;

/**@section Method */
public:
    [[nodiscard]] constexpr bool Intersect(const BasicRect& rhs) const noexcept;
    int32_t ToString(const std::span<char8_t>& destStr) const;
    int32_t ToString(char8_t* destStr, size_t destStrBufferLen) const;
    [[nodiscard]] std::u8string ToString() const;

/**@section Variable */
public:
    _Value x{}, y{}, width{}, height{};
};

using Rect = BasicRect<float>;
using DoubleRect = BasicRect<double>;
using IntRect = BasicRect<int32_t>;
using Int64Rect = BasicRect<int64_t>;

template <typename... _Types> requires Arithmetic<_Types...>
BasicRect(_Types...) -> BasicRect<std::common_type_t<_Types...>>;

template <typename _Value> requires Arithmetic<_Value>
constexpr BasicRect<_Value>::BasicRect(const _Value& x, const _Value& y, const _Value& width, const _Value& height) noexcept :
    x(x),
    y(y),
    width(width),
    height(height)
{
}

template <typename _Value> requires Arithmetic<_Value>
template <typename _Value2> requires Arithmetic<_Value>
constexpr BasicRect<_Value>::BasicRect(const BasicRect<_Value2>& rhs) noexcept :
    x(static_cast<_Value>(rhs.x)),
    y(static_cast<_Value>(rhs.y)),
    width(static_cast<_Value>(rhs.width)),
    height(static_cast<_Value>(rhs.height))
{
}

template <typename _Value> requires Arithmetic<_Value>
constexpr BasicRect<_Value> BasicRect<_Value>::operator-() const noexcept
{
    return BasicRect(-x, -y, -width, -height);
}

template <typename _Value> requires Arithmetic<_Value>
template <typename _Value2> requires Arithmetic<_Value>
BasicRect<_Value>& BasicRect<_Value>::operator=(const BasicRect<_Value2>& rhs) noexcept
{
    x = static_cast<_Value>(rhs.x);
    y = static_cast<_Value>(rhs.y);
    width = static_cast<_Value>(rhs.width);
    height = static_cast<_Value>(rhs.height);
    
    return *this;
}

template <typename _Value> requires Arithmetic<_Value>
constexpr bool BasicRect<_Value>::operator==(const BasicRect& rhs) const noexcept
{
    return (x == rhs.x && y == rhs.y && width == rhs.width && height == rhs.height);
}

template <typename _Value> requires Arithmetic<_Value>
constexpr bool BasicRect<_Value>::operator!=(const BasicRect& rhs) const noexcept
{
    return !(*this == rhs);
}

template <typename _Value> requires Arithmetic<_Value>
constexpr bool BasicRect<_Value>::Intersect(const BasicRect& rhs) const noexcept
{
    return (x <= rhs.width && y <= rhs.height && width >= rhs.x && height >= rhs.y);
}

template <typename _Value> requires Arithmetic<_Value>
int32_t BasicRect<_Value>::ToString(const std::span<char8_t>& destStr) const
{
    return this->ToString(&destStr[0], destStr.size());
}

template <typename _Value> requires Arithmetic<_Value>
std::u8string BasicRect<_Value>::ToString() const
{
    std::array<char8_t, 1024> str{};
    const int32_t strLen = this->ToString(str);

    return {&str[0], static_cast<size_t>(strLen)};
}

template <typename _Value> requires Arithmetic<_Value>
int32_t BasicRect<_Value>::ToString(char8_t* destStr, size_t destStrBufferLen) const
{
    const auto destStrLen = fmt::format_to_n(destStr, sizeof(destStr[0]) * (destStrBufferLen - 1), u8"{} {} {} {}", x, y, width, height).size;
    destStr[destStrLen] = u8'\0';

    return static_cast<int32_t>(destStrLen);
}

}
