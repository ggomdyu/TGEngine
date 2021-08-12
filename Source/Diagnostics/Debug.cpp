#include "PrecompiledHeader.h"

#include <algorithm>
#include <fmt/format.h>

#include "Debug.h"

namespace tg
{

void Debug::Assert(bool condition)
{
    Assert(condition, {u8"", 0}, {u8"", 0});
}

void Debug::Assert(bool condition, const std::u8string_view& message)
{
    Assert(condition, message, {u8"", 0});
}

void Debug::Fail(const std::u8string_view& message)
{
    Fail(message, {u8"", 0});
}

void Debug::Fail(const std::u8string_view& message, const std::u8string_view& detailMessage)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    const auto str = fmt::format(u8"---- DEBUG ASSERTION FAILED ----\n---- Assert Short Message ----\n{0}\n---- Assert Long Message ----\n{1}\n", message, detailMessage);
    
    std::lock_guard lockGuard(m_mutex);

    // Ignore the indent when printing the failure message
    const auto prevIndentLevel = m_indentLevel;
    m_indentLevel = 0;
    {
        Write(str);
    }
    m_indentLevel = prevIndentLevel;

    abort();
#endif
}

void Debug::SetIndentLevel(int32_t indentLevel) noexcept
{
#if defined(_DEBUG) || !defined(NDEBUG)
    std::lock_guard lockGuard(m_mutex);
    m_indentLevel = std::max(indentLevel, 0);
#endif
}

int32_t Debug::GetIndentLevel() noexcept
{
    return m_indentLevel;
}

void Debug::Indent()
{
#if defined(_DEBUG) || !defined(NDEBUG)
    std::lock_guard lockGuard(m_mutex);
    m_indentLevel += 1;
#endif
}

void Debug::Unindent()
{
#if defined(_DEBUG) || !defined(NDEBUG)
    std::lock_guard lockGuard(m_mutex);
    m_indentLevel = std::max(m_indentLevel - 1, 0);
#endif
}

}