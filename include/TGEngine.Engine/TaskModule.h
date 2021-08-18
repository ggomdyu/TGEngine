#pragma once

#include "Threading/DispatchQueue.h"

#include "Module.h"

TG_NAMESPACE_BEGIN

class TaskModule :
	public Module
{
public:
    TG_OBJECT(TaskModule)

public:
    explicit TaskModule(int32_t threadPoolCount = Thread::GetHardwareConcurrency());

public:
    SerialDispatchQueue& GetMainDispatchQueue() noexcept;
    const SerialDispatchQueue& GetMainDispatchQueue() const noexcept;
    ConcurrentDispatchQueue& GetGlobalDispatchQueue();
    const ConcurrentDispatchQueue& GetGlobalDispatchQueue() const;
    void Update() override;
    
public:
    static constexpr auto ModuleStage = ModuleStage::Update;

private:
    SerialDispatchQueue _mainDispatchQueue;
    ConcurrentDispatchQueue _globalDispatchQueue;
};

}
