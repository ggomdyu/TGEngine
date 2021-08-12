#pragma once

#include <thread>

#include "Platform/Config.h"

namespace tg
{

enum class ThreadPriority
{
    TimeCritical,
    Highest,
    Normal,
    Lowest
};

class Thread :
    private std::thread
{
/**@section Type */
public:
    using Super = std::thread;
    using ThreadId = std::thread::id;

/**@section Constructor */
public:
    using Super::Super;

/**@section Method */
public:
    void SetPriority(ThreadPriority threadPriority);
    void Detach();
    void Join();
    bool Joinable() const;
    static int32_t GetHardwareConcurrency() noexcept;
    static bool IsMainThread(ThreadId threadId) noexcept;
    ThreadId GetId() const noexcept;
    const void* GetNativeHandle() const;
    void* GetNativeHandle();
};

}
