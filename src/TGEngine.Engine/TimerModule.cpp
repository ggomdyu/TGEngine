#include "Platform/Environment.h"

#include "TimerModule.h"

TG_NAMESPACE_BEGIN

TimerModule::TimerModule() noexcept :
    _prevFrameTime(Environment::GetTickCount())
{
}

TimerHandle TimerModule::SetTimer(Delegate<void()> callback, float interval, bool isLoop)
{
    const auto timerHandle = CreateTimerHandle();

    _timerInfos.push_back(TimerInfo{
        .timerHandle = timerHandle,
        .callback = std::move(callback),
        .elapsedTime = 0.0f,
        .interval = interval,
        .isLoop = isLoop,
        .isDeleteReserved = false
    });
    
    return timerHandle;
}

float TimerModule::GetTimerElapsed(TimerHandle timerHandle) const noexcept
{
    auto predicate = [&](const TimerInfo& timerInfo)
    {
        return timerInfo.timerHandle == timerHandle;
    };

    const auto it = std::find_if(_timerInfos.cbegin(), _timerInfos.cend(), predicate);
    if (it == _timerInfos.cend())
    {
        return 0.0f;
    }
    
    return it->interval - it->elapsedTime;
}

bool TimerModule::ClearTimer(TimerHandle timerHandle)
{
    auto predicate = [&](const TimerInfo& timerInfo)
    {
        return timerInfo.timerHandle == timerHandle;
    };

    const auto it = std::find_if(_timerInfos.begin(), _timerInfos.end(), predicate);
    if (it != _timerInfos.end())
    {
        it->isDeleteReserved = true;
        return true;
    }

    return false;
}

TimerHandle TimerModule::CreateTimerHandle() noexcept
{
    static int64_t rawTimerHandle;
    return TimerHandle(++rawTimerHandle);
}

void TimerModule::Update()
{
    const auto currentFrameTime = Environment::GetTickCount();

    const auto tickTime = static_cast<float>(currentFrameTime - _prevFrameTime) * 0.001f;
    _prevFrameTime = currentFrameTime;
    
    for (auto it = _timerInfos.begin(); it != _timerInfos.end();)
    {
        auto& timerInfo = *it;
        if (timerInfo.elapsedTime >= timerInfo.interval)
        {
            if (timerInfo.isDeleteReserved == false)
            {
                timerInfo.callback();

                if (timerInfo.isLoop)
                {
                    timerInfo.elapsedTime = 0.0f;
                }
                else
                {
                    it = _timerInfos.erase(it);
                    continue;
                }
            }
            else
            {
                it = _timerInfos.erase(it);
                continue;
            }
        }
        else
        {
            timerInfo.elapsedTime += tickTime;
        }
        
        ++it;
    }
}

}
