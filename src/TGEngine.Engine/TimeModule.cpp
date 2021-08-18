#include "Platform/Environment.h"

#include "TimeModule.h"

TG_NAMESPACE_BEGIN

TimeModule::TimeModule() :
    _prevFrameTime(Environment::GetTickCount())
{
}

void TimeModule::SetTimeScale(float timeScale) noexcept
{
    _timeScale = timeScale;
}

float TimeModule::GetTimeScale() const noexcept
{
    return _timeScale;
}

float TimeModule::GetDeltaTime() const noexcept
{
    return _deltaTime * _timeScale;
}

float TimeModule::GetUnscaledDeltaTime() const noexcept
{
    return _deltaTime;
}

float TimeModule::GetSmoothDeltaTime() const noexcept
{
    return _smoothDeltaTime * _timeScale;
}

int32_t TimeModule::GetFrameCount() const noexcept
{
    return _frameCount;
}

void TimeModule::Update()
{
    const auto currentFrameTime = Environment::GetTickCount();

    _deltaTime = static_cast<float>(currentFrameTime - _prevFrameTime) * 0.001f;
    _smoothDeltaTime = SmoothDeltaTimeFactor * _deltaTime + (1.0f - SmoothDeltaTimeFactor) * _smoothDeltaTime;

    ++_frameCount;

    _prevFrameTime = currentFrameTime;
}

}
