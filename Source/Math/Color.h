#pragma once

#include <array>
#include <fmt/format.h>
#include <span>

#include "Core/ExpressionTemplates.h"

#include "Mathf.h"

namespace tg
{

struct Color final
{
/**@section Constructor */
public:
    constexpr Color() noexcept = default;
    constexpr Color(float r, float g, float b, float a) noexcept;
    constexpr Color(float r, float g, float b) noexcept;

/**@section Operator */
public:
    constexpr ExpressionTemplates<Add, Color, Color> operator+(const Color& rhs) const noexcept;
    constexpr ExpressionTemplates<Subtract, Color, Color> operator-(const Color& rhs) const noexcept;
    constexpr ExpressionTemplates<Subtract, Color, Color> operator*(const Color& rhs) const noexcept;
    constexpr ExpressionTemplates<Multiply, Color, float> operator*(const float& rhs) const noexcept;
    constexpr ExpressionTemplates<Divide, Color, float> operator/(const float& rhs) const;
    Color& operator+=(const Color& rhs) noexcept;
    Color& operator-=(const Color& rhs) noexcept;
    Color& operator*=(const Color& rhs) noexcept;
    Color& operator*=(float rhs) noexcept;
    Color& operator/=(float rhs);
    float& operator[](int32_t index);
    float operator[](int32_t index) const;
    constexpr bool operator==(const Color& rhs) const noexcept;
    constexpr bool operator!=(const Color& rhs) const noexcept;

/**@section Method */
public:
    [[nodiscard]] static constexpr Color Black() noexcept;
    [[nodiscard]] static constexpr Color Blue() noexcept;
    [[nodiscard]] static constexpr Color Clear() noexcept;
    [[nodiscard]] static constexpr Color Cyan() noexcept;
    [[nodiscard]] static constexpr Color Gray() noexcept;
    [[nodiscard]] static constexpr Color Grey() noexcept;
    [[nodiscard]] static constexpr Color Green() noexcept;
    [[nodiscard]] static constexpr Color Magenta() noexcept;
    [[nodiscard]] static constexpr Color Red() noexcept;
    [[nodiscard]] static constexpr Color White() noexcept;
    [[nodiscard]] static constexpr Color Yellow() noexcept;
    [[nodiscard]] constexpr float Grayscale() const noexcept;
    [[nodiscard]] constexpr float GetMaxColorComponent() const noexcept;
    int32_t ToString(const std::span<char8_t>& destStr) const;
    int32_t ToString(char8_t* destStr, size_t destStrBufferLen) const;
    [[nodiscard]] std::u8string ToString() const;

/**@section Variable */
public:
    float r{}, g{}, b{}, a{};
};

constexpr Color::Color(float r, float g, float b, float a) noexcept :
    r(r),
    g(g),
    b(b),
    a(a)
{
}

constexpr Color::Color(float r, float g, float b) noexcept :
    r(r),
    g(g),
    b(b),
    a(1.0f)
{
}

constexpr ExpressionTemplates<Add, Color, Color> Color::operator+(const Color& rhs) const noexcept
{
    return {*this, rhs};
}

constexpr ExpressionTemplates<Subtract, Color, Color> Color::operator-(const Color& rhs) const noexcept
{
    return {*this, rhs};
}

constexpr ExpressionTemplates<Subtract, Color, Color> Color::operator*(const Color& rhs) const noexcept
{
    return {*this, rhs};
}

constexpr ExpressionTemplates<Multiply, Color, float> Color::operator*(const float& rhs) const noexcept
{
    return {*this, rhs};
}

constexpr ExpressionTemplates<Multiply, Color, float> operator*(const float& lhs, const Color& rhs) noexcept
{
    return {rhs, lhs};
}

constexpr ExpressionTemplates<Divide, Color, float> Color::operator/(const float& rhs) const
{
    return {*this, rhs};
}

inline float& Color::operator[](int32_t index)
{
    return *(&r + index);
}

inline float Color::operator[](int32_t index) const
{
    return *(&r + index);
}

constexpr bool Color::operator==(const Color& rhs) const noexcept
{
    return r == rhs.r && g == rhs.g && b == rhs.b && a == rhs.a;
}

constexpr bool Color::operator!=(const Color& rhs) const noexcept
{
    return !this->operator==(rhs);
}

constexpr Color Color::Black() noexcept
{
    return {0.0f, 0.0f, 0.0f, 1.0f};
}

constexpr Color Color::Blue() noexcept
{
    return {0.0f, 0.0f, 1.0f, 1.0f};
}

constexpr Color Color::Clear() noexcept
{
    return {0.0f, 0.0f, 0.0f, 0.0f};
}

constexpr Color Color::Cyan() noexcept
{
    return {0.0f, 1.0f, 1.0f, 1.0f};
}

constexpr Color Color::Gray() noexcept
{
    return {0.5f, 0.5f, 0.5f, 1.0f};
}

constexpr Color Color::Grey() noexcept
{
    return Gray();
}

constexpr Color Color::Green() noexcept
{
    return {0.0f, 1.0f, 0.0f, 1.0f};
}

constexpr Color Color::Magenta() noexcept
{
    return {1.0f, 0.0f, 1.0f, 1.0f};
}

constexpr Color Color::Red() noexcept
{
    return {1.0f, 0.0f, 0.0f, 1.0f};
}

constexpr Color Color::White() noexcept
{
    return {1.0f, 1.0f, 1.0f, 1.0f};
}

constexpr Color Color::Yellow() noexcept
{
    return {1.0f, 0.92f, 0.016f, 1.0f};
}

constexpr float Color::Grayscale() const noexcept
{
    return static_cast<float>(0.29899999499321 * static_cast<double>(r) + 0.587000012397766 * static_cast<double>(g) + 57.0 / 500.0 * static_cast<double>(b));
}

constexpr float Color::GetMaxColorComponent() const noexcept
{
    return Mathf::Max(r, g, b, a);
}

inline Color& Color::operator+=(const Color& rhs) noexcept
{
    r += rhs.r;
    g += rhs.g;
    b += rhs.b;
    a += rhs.a;

    return *this;
}

inline Color& Color::operator-=(const Color& rhs) noexcept
{
    r -= rhs.r;
    g -= rhs.g;
    b -= rhs.b;
    a -= rhs.a;

    return *this;
}

inline Color& Color::operator*=(const Color& rhs) noexcept
{
    r *= rhs.r;
    g *= rhs.g;
    b *= rhs.b;
    a *= rhs.a;

    return *this;
}

inline Color& Color::operator*=(float rhs) noexcept
{
    r *= rhs;
    g *= rhs;
    b *= rhs;
    a *= rhs;

    return *this;
}

inline Color& Color::operator/=(float rhs)
{
    r /= rhs;
    g /= rhs;
    b /= rhs;
    a /= rhs;

    return *this;
}

inline int32_t Color::ToString(const std::span<char8_t>& destStr) const
{
    return this->ToString(&destStr[0], destStr.size());
}

inline std::u8string Color::ToString() const
{
    std::array<char8_t, 1024> str{};
    const int32_t strLen = this->ToString(str);

    return {&str[0], static_cast<size_t>(strLen)};
}

inline int32_t Color::ToString(char8_t* destStr, size_t destStrBufferLen) const
{
    const auto destStrLen = fmt::format_to_n(destStr, sizeof(destStr[0]) * (destStrBufferLen - 1), "{} {} {} {}", r, g, b, a).size;
    destStr[destStrLen] = u8'\0';

    return static_cast<int32_t>(destStrLen);
}

}