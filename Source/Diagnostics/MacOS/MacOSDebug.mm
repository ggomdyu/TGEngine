#include "PrecompiledHeader.h"

#include <string>
#include <mutex>
#include <AppKit/NSAlert.h>

#include "../Debug.h"

namespace tg
{

void Debug::Write(const std::u8string_view& message)
{
#if defined(_DEBUG) || !defined(NDEBUG)
	std::u8string str(message);
    str.insert(0, m_indentLevel, u8'\t');

    std::lock_guard<std::recursive_mutex> lockGuard(m_mutex);
    printf("%s", reinterpret_cast<const char*>(str.c_str()));
#endif
}

void Debug::WriteLine(const std::u8string_view& message)
{
#if defined(_DEBUG) || !defined(NDEBUG)
    std::u8string str(message);
    str.insert(0, m_indentLevel, u8'\t');
    str += u8"\n";

    std::lock_guard<std::recursive_mutex> lockGuard(m_mutex);
    printf("%s", reinterpret_cast<const char*>(str.c_str()));
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
