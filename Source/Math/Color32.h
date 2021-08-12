#pragma once

#include <array>
#include <fmt/format.h>
#include <span>

namespace tg
{

struct Color32 final
{
/**@section Constructor */
public:
    constexpr Color32() noexcept = default;
    constexpr Color32(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept;
    constexpr Color32(uint32_t color) noexcept;

/**@section Operator */
public:
    uint8_t& operator[](int32_t index);
    uint8_t operator[](int32_t index) const;
    constexpr bool operator==(const Color32& rhs) const noexcept;
    constexpr bool operator!=(const Color32& rhs) const noexcept;
    constexpr operator uint32_t() const noexcept;
    
/**@section Method */
public:
    int32_t ToString(const std::span<char8_t>& destStr) const;
    int32_t ToString(char8_t* destStr, size_t destStrBufferLen) const;
    [[nodiscard]] std::u8string ToString() const;

/**@section Variable */
public:
    union
    {
        uint32_t color{};
        struct { uint8_t r, g, b, a; };
    };
};

constexpr Color32::Color32(uint8_t r, uint8_t g, uint8_t b, uint8_t a) noexcept :
    r(r),
    g(g),
    b(b),
    a(a)
{
}

constexpr Color32::Color32(uint32_t color) noexcept :
    color(color)
{
}

constexpr bool Color32::operator==(const Color32& rhs) const noexcept
{
    return color == rhs.color;
}

constexpr bool Color32::operator!=(const Color32& rhs) const noexcept
{
    return !this->operator==(rhs);
}

constexpr Color32::operator uint32_t() const noexcept
{
    return color;
}

inline uint8_t& Color32::operator[](int32_t index)
{
    return *(&r + index);
}

inline uint8_t Color32::operator[](int32_t index) const
{
    return *(&r + index);
}

inline int32_t Color32::ToString(const std::span<char8_t>& destStr) const
{
    return this->ToString(&destStr[0], destStr.size());
}

inline std::u8string Color32::ToString() const
{
    std::array<char8_t, 1024> str{};
    const int32_t strLen = this->ToString(str);

    return {&str[0], static_cast<size_t>(strLen)};
}

inline int32_t Color32::ToString(char8_t* destStr, size_t destStrBufferLen) const
{
    const auto destStrLen = fmt::format_to_n(destStr, sizeof(destStr[0]) * (destStrBufferLen - 1), "{} {} {} {}", r, g, b, a).size;
    destStr[destStrLen] = u8'\0';

    return static_cast<int32_t>(destStrLen);
}

}