#include "PrecompiledHeader.h"

#include "Platform/Windows/Windows.h"

#include "../Thread.h"

namespace tg
{

constexpr int ConvertThreadPriorityToNative(ThreadPriority threadPriority) noexcept
{
    constexpr int nativeThreadPriorities[] = {
        THREAD_PRIORITY_TIME_CRITICAL,
        THREAD_PRIORITY_HIGHEST,
        THREAD_PRIORITY_NORMAL,
        THREAD_PRIORITY_LOWEST
    };

    return nativeThreadPriorities[static_cast<int>(threadPriority)];
}

void Thread::SetPriority(ThreadPriority threadPriority)
{
    HANDLE threadHandle = OpenThread(THREAD_ALL_ACCESS, TRUE, GetCurrentThreadId());
    if (threadHandle != nullptr)
    {
        // Should we have to disable the boost option?
        //SetThreadPriorityBoost(threadHandle, TRUE);
        SetThreadPriority(threadHandle, ConvertThreadPriorityToNative(threadPriority));
    }
}

}
