#include "PrecompiledHeader.h"

#include "Platform/Environment.h"

#include "TimeModule.h"

namespace tg
{

TimeModule::TimeModule() :
    m_prevFrameTime(Environment::GetTickCount())
{
}

void TimeModule::SetTimeScale(float timeScale) noexcept
{
    m_timeScale = timeScale;
}

float TimeModule::GetTimeScale() const noexcept
{
    return m_timeScale;
}

float TimeModule::GetDeltaTime() const noexcept
{
    return m_deltaTime * m_timeScale;
}

float TimeModule::GetUnscaledDeltaTime() const noexcept
{
    return m_deltaTime;
}

float TimeModule::GetSmoothDeltaTime() const noexcept
{
    return m_smoothDeltaTime * m_timeScale;
}

int32_t TimeModule::GetFrameCount() const noexcept
{
    return m_frameCount;
}

void TimeModule::Update()
{
    const auto currentFrameTime = Environment::GetTickCount();

    m_deltaTime = static_cast<float>(currentFrameTime - m_prevFrameTime) * 0.001f;
    m_smoothDeltaTime = SmoothDeltaTimeFactor * m_deltaTime + (1.0f - SmoothDeltaTimeFactor) * m_smoothDeltaTime;

    ++m_frameCount;

    m_prevFrameTime = currentFrameTime;
}

}
