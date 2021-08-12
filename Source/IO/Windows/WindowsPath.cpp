#include "PrecompiledHeader.h"

#include "../Path.h"

namespace tg
{

std::span<const char8_t> Path::GetInvalidFileNameChars() noexcept
{
    constexpr char8_t invalidFileNameChars[] = {
        '\"', '<', '>', '|', '\0', static_cast<char8_t>(1), static_cast<char8_t>(2), static_cast<char8_t>(3),
        static_cast<char8_t>(4), static_cast<char8_t>(5), static_cast<char8_t>(6), static_cast<char8_t>(7), static_cast<char8_t>(8),
        static_cast<char8_t>(9), static_cast<char8_t>(10), static_cast<char8_t>(11), static_cast<char8_t>(12),
        static_cast<char8_t>(13), static_cast<char8_t>(14), static_cast<char8_t>(15), static_cast<char8_t>(16),
        static_cast<char8_t>(17), static_cast<char8_t>(18), static_cast<char8_t>(19), static_cast<char8_t>(20),
        static_cast<char8_t>(21), static_cast<char8_t>(22), static_cast<char8_t>(23), static_cast<char8_t>(24),
        static_cast<char8_t>(25), static_cast<char8_t>(26), static_cast<char8_t>(27), static_cast<char8_t>(28),
        static_cast<char8_t>(29), static_cast<char8_t>(30), static_cast<char8_t>(31), ':', '*', '?', '\\', '/'
    };
    return std::span(invalidFileNameChars, std::extent_v<decltype(invalidFileNameChars)> - 1);
}

std::span<const char8_t> Path::GetInvalidPathChars() noexcept
{
    constexpr char8_t invalidPathChars[] = {
        '|', '\0', static_cast<char8_t>(1), static_cast<char8_t>(2), static_cast<char8_t>(3), static_cast<char8_t>(4),
        static_cast<char8_t>(5), static_cast<char8_t>(6), static_cast<char8_t>(7), static_cast<char8_t>(8), static_cast<char8_t>(9),
        static_cast<char8_t>(10), static_cast<char8_t>(11), static_cast<char8_t>(12), static_cast<char8_t>(13),
        static_cast<char8_t>(14), static_cast<char8_t>(15), static_cast<char8_t>(16), static_cast<char8_t>(17),
        static_cast<char8_t>(18), static_cast<char8_t>(19), static_cast<char8_t>(20), static_cast<char8_t>(21),
        static_cast<char8_t>(22), static_cast<char8_t>(23), static_cast<char8_t>(24), static_cast<char8_t>(25),
        static_cast<char8_t>(26), static_cast<char8_t>(27), static_cast<char8_t>(28), static_cast<char8_t>(29),
        static_cast<char8_t>(30), static_cast<char8_t>(31)
    };
    return std::span(invalidPathChars, std::extent_v<decltype(invalidPathChars)> - 1);
}

}
