#pragma once

#include <cstdint>
#include <fmt/format.h>
#include <span>
#include <string>
#include <type_traits>

#include "Core/Concepts.h"
#include "Core/ExpressionTemplates.h"

namespace tg
{

template <typename _Value> requires Arithmetic<_Value>
struct BasicExtent2D
{
/**@section Type */
public:
    using ValueType = _Value;

/**@section Constructor */
public:
    constexpr BasicExtent2D() noexcept = default;
    constexpr BasicExtent2D(const _Value& width, const _Value& height) noexcept;
    template <typename _Value2> requires Arithmetic<_Value>
    constexpr BasicExtent2D(const BasicExtent2D<_Value2>& extent) noexcept;
    template <typename _Operator, typename _FirstOperand, typename _SecondOperand>
    constexpr BasicExtent2D(const ExpressionTemplates<_Operator, _FirstOperand, _SecondOperand>& expression);

/**@section Operator */
public:
    constexpr ExpressionTemplates<Add, BasicExtent2D, BasicExtent2D> operator+(const BasicExtent2D& rhs) const noexcept;
    constexpr ExpressionTemplates<Subtract, BasicExtent2D, BasicExtent2D> operator-(const BasicExtent2D& rhs) const noexcept;
    constexpr ExpressionTemplates<Multiply, BasicExtent2D, _Value> operator*(const _Value& rhs) const noexcept;
    constexpr ExpressionTemplates<Divide, BasicExtent2D, _Value> operator/(const _Value& rhs) const;
    constexpr BasicExtent2D operator-() const noexcept;
    BasicExtent2D& operator+=(const BasicExtent2D& rhs) noexcept;
    BasicExtent2D& operator-=(const BasicExtent2D& rhs) noexcept;
    BasicExtent2D& operator*=(const _Value& rhs) noexcept;
    BasicExtent2D& operator/=(const _Value& rhs);
    template <typename _Value2> requires Arithmetic<_Value>
    BasicExtent2D& operator=(const BasicExtent2D<_Value2>& rhs) noexcept;
    constexpr bool operator==(const BasicExtent2D& rhs) const noexcept;
    constexpr bool operator!=(const BasicExtent2D& rhs) const noexcept;

/**@section Method */
public:
    int32_t ToString(const std::span<char8_t>& destStr) const;
    int32_t ToString(char8_t* destStr, size_t destStrBufferLen) const;
    [[nodiscard]] std::u8string ToString() const;

/**@section Variable */
public:
    _Value width{}, height{};
};

using FExtent2D = BasicExtent2D<float>;
using DExtent2D = BasicExtent2D<double>;
using I32Extent2D = BasicExtent2D<int32_t>;
using I64Extent2D = BasicExtent2D<int64_t>;

template <typename... _Types> requires Arithmetic<_Types...>
BasicExtent2D(_Types...) -> BasicExtent2D<std::common_type_t<_Types...>>;

template <typename _Value> requires Arithmetic<_Value>
constexpr BasicExtent2D<_Value>::BasicExtent2D(const _Value& width, const _Value& height) noexcept :
    width(width),
    height(height)
{
}

template <typename _Value> requires Arithmetic<_Value>
template <typename _Value2> requires Arithmetic<_Value>
constexpr BasicExtent2D<_Value>::BasicExtent2D(const BasicExtent2D<_Value2>& extent) noexcept :
    width(static_cast<_Value>(extent.width)),
    height(static_cast<_Value>(extent.height))
{
}

template <typename _Value> requires Arithmetic<_Value>
template <typename _Operator, typename _FirstOperand, typename _SecondOperand>
constexpr BasicExtent2D<_Value>::BasicExtent2D(const ExpressionTemplates<_Operator, _FirstOperand, _SecondOperand>& expression) :
    BasicExtent2D(expression[0], expression[1])
{
}

template <typename _Value> requires Arithmetic<_Value>
constexpr ExpressionTemplates<Add, BasicExtent2D<_Value>, BasicExtent2D<_Value>> BasicExtent2D<_Value>::operator+(const BasicExtent2D& rhs) const noexcept
{
    return {*this, rhs};
}

template <typename _Value> requires Arithmetic<_Value>
constexpr ExpressionTemplates<Subtract, BasicExtent2D<_Value>, BasicExtent2D<_Value>> BasicExtent2D<_Value>::operator-(const BasicExtent2D& rhs) const noexcept
{
    return {*this, rhs};
}

template <typename _Value> requires Arithmetic<_Value>
constexpr ExpressionTemplates<Multiply, BasicExtent2D<_Value>, _Value> BasicExtent2D<_Value>::operator*(const _Value& rhs) const noexcept
{
    return {*this, rhs};
}

template <typename _Value> requires Arithmetic<_Value>
constexpr ExpressionTemplates<Divide, BasicExtent2D<_Value>, _Value> BasicExtent2D<_Value>::operator/(const _Value& rhs) const
{
    return {*this, rhs};
}

template <typename _Value> requires Arithmetic<_Value>
constexpr ExpressionTemplates<Divide, _Value, BasicExtent2D<_Value>> operator*(const _Value& lhs, const BasicExtent2D<_Value>& rhs)
{
    return {lhs, rhs};
}

template <typename _Value> requires Arithmetic<_Value>
constexpr BasicExtent2D<_Value> BasicExtent2D<_Value>::operator-() const noexcept
{
    return BasicExtent2D(-width, -height);
}

template <typename _Value> requires Arithmetic<_Value>
BasicExtent2D<_Value>& BasicExtent2D<_Value>::operator+=(const BasicExtent2D& rhs) noexcept
{
    width += rhs.width;
    height += rhs.height;

    return *this;
}

template <typename _Value> requires Arithmetic<_Value>
BasicExtent2D<_Value>& BasicExtent2D<_Value>::operator-=(const BasicExtent2D& rhs) noexcept
{
    width -= rhs.width;
    height -= rhs.height;

    return *this;
}

template <typename _Value> requires Arithmetic<_Value>
BasicExtent2D<_Value>& BasicExtent2D<_Value>::operator*=(const _Value& rhs) noexcept
{
    width *= rhs;
    height *= rhs;

    return *this;
}

template <typename _Value> requires Arithmetic<_Value>
BasicExtent2D<_Value>& BasicExtent2D<_Value>::operator/=(const _Value& rhs)
{
    width /= rhs;
    height /= rhs;

    return *this;
}

template <typename _Value> requires Arithmetic<_Value>
template <typename _Value2> requires Arithmetic<_Value>
BasicExtent2D<_Value>& BasicExtent2D<_Value>::operator=(const BasicExtent2D<_Value2>& rhs) noexcept
{
    width = rhs.width;
    height = rhs.height;

    return *this;
}

template <typename _Value> requires Arithmetic<_Value>
constexpr bool BasicExtent2D<_Value>::operator==(const BasicExtent2D& rhs) const noexcept
{
    return (width == rhs.width && height == rhs.height);
}

template <typename _Value> requires Arithmetic<_Value>
constexpr bool BasicExtent2D<_Value>::operator!=(const BasicExtent2D& rhs) const noexcept
{
    return !(*this == rhs);
}

template <typename _Value> requires Arithmetic<_Value>
int32_t BasicExtent2D<_Value>::ToString(const std::span<char8_t>& destStr) const
{
    return this->ToString(&destStr[0], destStr.size());
}

template <typename _Value> requires Arithmetic<_Value>
std::u8string BasicExtent2D<_Value>::ToString() const
{
    std::array<char8_t, 1024> str;
    const int32_t strLen = this->ToString(str);

    return {&str[0], static_cast<size_t>(strLen)};
}

template <typename _Value> requires Arithmetic<_Value>
int32_t BasicExtent2D<_Value>::ToString(char8_t* destStr, size_t destStrBufferLen) const
{
    auto destStrLen = fmt::format_to_n(destStr, sizeof(destStr[0]) * (destStrBufferLen - 1), u8"{} {}", width, height).size;
    destStr[destStrLen] = u8'\0';

    return static_cast<int32_t>(destStrLen);
}

}
