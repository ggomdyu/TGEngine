#pragma once

#include <vector>

#include "Core/Delegate.h"

#include "Module.h"

namespace tg
{

using TimerHandle = int64_t;

class TimerModule :
	public Module
{
public:
    TGON_RTTI(TimerModule)

/**@section Struct */
private:
    struct TimerInfo
    {
        TimerHandle timerHandle;
        Delegate<void()> callback;
        float elapsedTime = 0.0f;
        float interval = 0.0f;
        bool isLoop = false;
        bool isDeleteReserved = false;
    };

/**@section Constructor */
public:
    TimerModule() noexcept;

/**@section Method */
public:
    void Update() override;
    TimerHandle SetTimer(Delegate<void()> callback, float interval, bool isLoop);
    bool ClearTimer(TimerHandle timerHandle);
    [[nodiscard]] float GetTimerElapsed(TimerHandle timerHandle) const noexcept;

private:
    static TimerHandle CreateTimerHandle() noexcept;
    
/**@section Variable */
public:
    static constexpr auto ModuleStage = ModuleStage::Update;

private:
    int64_t m_prevFrameTime;
    std::vector<TimerInfo> m_timerInfos;
};

}
