#pragma once

#include "Module.h"

TG_NAMESPACE_BEGIN

class TimeModule final :
	public Module
{
public:
    TG_OBJECT(TimeModule)

public:
    TimeModule();

public:
    void Update() override;
    void SetTimeScale(float timeScale) noexcept;
    float GetTimeScale() const noexcept;
    float GetDeltaTime() const noexcept;
    float GetUnscaledDeltaTime() const noexcept;
    float GetSmoothDeltaTime() const noexcept;
    int32_t GetFrameCount() const noexcept;

public:
    static constexpr auto ModuleStage = ModuleStage::Update;

private:
    int64_t _prevFrameTime = 1.0f;
    float _timeScale = 1.0f;
    float _deltaTime = 0.0f;
    float _smoothDeltaTime = 0.0f;
    int32_t _frameCount = 0;
    static constexpr float SmoothDeltaTimeFactor = 0.2f;
};

}
