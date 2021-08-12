#include "PrecompiledHeader.h"

#include <array>

#include "Platform/Windows/Windows.h"
#include "Text/Encoding.h"

#include "../Debug.h"

namespace tg
{

void Debug::Write(const std::u8string_view& message)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    auto utf16MessageBytes = Encoding::Convert(Encoding::UTF8(), Encoding::Unicode(), reinterpret_cast<const std::byte*>(message.data()), static_cast<int32_t>(message.size()));
    if (utf16MessageBytes.has_value() == false)
    {
        return;
    }

    for (auto i = 0; i < m_indentLevel; ++i)
    {
        utf16MessageBytes->insert(utf16MessageBytes->end(), {std::byte('\t'), std::byte('\0')});
    }
    utf16MessageBytes->insert(utf16MessageBytes->end(), {std::byte('\0'), std::byte('\0')});

    std::lock_guard lockGuard(m_mutex);
    OutputDebugStringW(reinterpret_cast<const wchar_t*>(utf16MessageBytes->data()));
#endif
}

void Debug::WriteLine(const std::u8string_view& message)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    auto utf16MessageBytes = Encoding::Convert(Encoding::UTF8(), Encoding::Unicode(), reinterpret_cast<const std::byte*>(message.data()), static_cast<int32_t>(message.size()));
    if (utf16MessageBytes.has_value() == false)
    {
        return;
    }

    for (auto i = 0; i < m_indentLevel; ++i)
    {
        utf16MessageBytes->insert(utf16MessageBytes->end(), {std::byte('\t'), std::byte('\0')});
    }
    utf16MessageBytes->insert(utf16MessageBytes->end(), {std::byte('\n'), std::byte('\0'), std::byte('\0'), std::byte('\0')});

    std::lock_guard lockGuard(m_mutex);
    OutputDebugStringW(reinterpret_cast<const wchar_t*>(utf16MessageBytes->data()));
#endif
}

void Debug::Assert(bool condition, const std::u8string_view& message, const std::u8string_view& detailMessage)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    if (condition)
    {
        return;
    }

    Fail(message, detailMessage);
#endif
}

}
