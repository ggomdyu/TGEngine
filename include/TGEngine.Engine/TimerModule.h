#pragma once

#include <vector>

#include "Core/Delegate.h"

#include "Module.h"

TG_NAMESPACE_BEGIN

using TimerHandle = int64_t;

class TimerModule :
	public Module
{
public:
    TG_OBJECT(TimerModule)

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

public:
    TimerModule() noexcept;

public:
    void Update() override;
    TimerHandle SetTimer(Delegate<void()> callback, float interval, bool isLoop);
    bool ClearTimer(TimerHandle timerHandle);
    float GetTimerElapsed(TimerHandle timerHandle) const noexcept;

private:
    static TimerHandle CreateTimerHandle() noexcept;
    
public:
    static constexpr auto ModuleStage = ModuleStage::Update;

private:
    int64_t _prevFrameTime;
    std::vector<TimerInfo> _timerInfos;
};

}
