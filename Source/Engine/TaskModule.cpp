#include "PrecompiledHeader.h"

#include "TaskModule.h"

namespace tg
{

TaskModule::TaskModule(int32_t threadPoolCount) :
    m_globalDispatchQueue(threadPoolCount)
{
}

SerialDispatchQueue& TaskModule::GetMainDispatchQueue() noexcept
{
    return m_mainDispatchQueue;
}

const SerialDispatchQueue& TaskModule::GetMainDispatchQueue() const noexcept
{
    return m_mainDispatchQueue;
}

ConcurrentDispatchQueue& TaskModule::GetGlobalDispatchQueue()
{
    return m_globalDispatchQueue;
}
    
const ConcurrentDispatchQueue& TaskModule::GetGlobalDispatchQueue() const
{
    return const_cast<TaskModule*>(this)->GetGlobalDispatchQueue();
}

void TaskModule::Update()
{
    m_mainDispatchQueue.Dispatch();
}

}
