#include "PrecompiledHeader.h"

#include <array>

#include "Window.h"

namespace tg
{

Window::~Window()
{
    this->Close();
}

PlatformWindow& Window::GetPlatformDependency() noexcept
{
    return *this;
}

const PlatformWindow& Window::GetPlatformDependency() const noexcept
{
    return *this;
}

std::u8string Window::GetTitle() const
{
    std::array<char8_t, 8192> str{};
    const int32_t titleLen = this->GetTitle(&str[0], static_cast<int32_t>(str.size()));

    return {&str[0], static_cast<size_t>(titleLen)};
}

}
