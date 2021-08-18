#include "TaskModule.h"

TG_NAMESPACE_BEGIN

TaskModule::TaskModule(int32_t threadPoolCount) :
    _globalDispatchQueue(threadPoolCount)
{
}

SerialDispatchQueue& TaskModule::GetMainDispatchQueue() noexcept
{
    return _mainDispatchQueue;
}

const SerialDispatchQueue& TaskModule::GetMainDispatchQueue() const noexcept
{
    return _mainDispatchQueue;
}

ConcurrentDispatchQueue& TaskModule::GetGlobalDispatchQueue()
{
    return _globalDispatchQueue;
}
    
const ConcurrentDispatchQueue& TaskModule::GetGlobalDispatchQueue() const
{
    return const_cast<TaskModule*>(this)->GetGlobalDispatchQueue();
}

void TaskModule::Update()
{
    _mainDispatchQueue.Dispatch();
}

}
