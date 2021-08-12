#include "PrecompiledHeader.h"

#include "Platform/Environment.h"

#include "TimerModule.h"

namespace tg
{

TimerModule::TimerModule() noexcept :
    m_prevFrameTime(Environment::GetTickCount())
{
}

TimerHandle TimerModule::SetTimer(Delegate<void()> callback, float interval, bool isLoop)
{
    const auto timerHandle = CreateTimerHandle();

    m_timerInfos.push_back(TimerInfo{
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

    const auto it = std::find_if(m_timerInfos.cbegin(), m_timerInfos.cend(), predicate);
    if (it == m_timerInfos.cend())
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

    const auto it = std::find_if(m_timerInfos.begin(), m_timerInfos.end(), predicate);
    if (it != m_timerInfos.end())
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

    const auto tickTime = static_cast<float>(currentFrameTime - m_prevFrameTime) * 0.001f;
    m_prevFrameTime = currentFrameTime;
    
    for (auto it = m_timerInfos.begin(); it != m_timerInfos.end();)
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
                    it = m_timerInfos.erase(it);
                    continue;
                }
            }
            else
            {
                it = m_timerInfos.erase(it);
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
