#include "PrecompiledHeader.h"

#include "DispatchQueue.h"
#include "Thread.h"

namespace tg
{
    
void SerialDispatchQueue::AddAsyncTask(const Delegate<void()>& task)
{
    std::lock_guard lock(m_mutex);

    m_taskPool.push_back(task);
}

void SerialDispatchQueue::AddAsyncTask(Delegate<void()>&& task)
{
    std::lock_guard lock(m_mutex);

    m_taskPool.push_back(std::move(task));
}

void SerialDispatchQueue::AddSyncTask(const Delegate<void()>& task)
{
    if (Thread::IsMainThread(std::this_thread::get_id()))
    {
        task();
    }
    else
    {
        bool isTaskExecuted = false;
        this->AddAsyncTask([&, task]()
        {
            task();
            isTaskExecuted = true;
        });
        while (isTaskExecuted == false);
    }
}

void SerialDispatchQueue::AddSyncTask(Delegate<void()>&& task)
{
    if (Thread::IsMainThread(std::this_thread::get_id()))
    {
        task();
    }
    else
    {
        bool isTaskExecuted = false;
        this->AddAsyncTask([&isTaskExecuted, task = std::move(task)]()
        {
            task();
            isTaskExecuted = true;
        });

        while (isTaskExecuted == false);
    }
}

void SerialDispatchQueue::Dispatch()
{
    for (auto& task : m_taskPool)
    {
        task();
    }
    
    m_taskPool.clear();
}

ConcurrentDispatchQueue::ConcurrentDispatchQueue(int32_t threadPoolCount) :
    m_needToDestroy(false)
{
    m_threadPool.reserve(threadPoolCount);
    for (int32_t i = 0; i < threadPoolCount; ++i)
    {
        m_threadPool.emplace_back(&ConcurrentDispatchQueue::DispatchQueueHandler, this);
    }
}

ConcurrentDispatchQueue::~ConcurrentDispatchQueue()
{
    m_needToDestroy = true;
    m_cv.notify_all();
    
    for (auto& thread : m_threadPool)
    {
        if (thread.Joinable())
        {
            thread.Join();
        }
    }
}
    
void ConcurrentDispatchQueue::AddAsyncTask(const Delegate<void()>& task)
{
    std::lock_guard lock(m_mutex);

    m_taskPool.push_back(task);
    m_cv.notify_one();
}

void ConcurrentDispatchQueue::AddAsyncTask(Delegate<void()>&& task)
{
    std::lock_guard lock(m_mutex);
    
    m_taskPool.push_back(std::move(task));
    m_cv.notify_one();
}
    
void ConcurrentDispatchQueue::AddSyncTask(const Delegate<void()>& task)
{
    bool isTaskExecuted = false;
    this->AddAsyncTask([&isTaskExecuted, &task]()
    {
        task();
        isTaskExecuted = true;
    });
    
    while (isTaskExecuted == false);
}

void ConcurrentDispatchQueue::AddSyncTask(Delegate<void()>&& task)
{
    bool isTaskExecuted = false;
    this->AddAsyncTask([&isTaskExecuted, task = std::move(task)]()
    {
        task();
        isTaskExecuted = true;
    });

    while (isTaskExecuted == false);
}

void ConcurrentDispatchQueue::DispatchQueueHandler()
{
    std::unique_lock lock(m_mutex);
    
    while (m_needToDestroy == false)
    {
        m_cv.wait(lock, [this]() {
            return (m_taskPool.size() > 0) || m_needToDestroy;
        });

        if ((m_taskPool.size() <= 0) || m_needToDestroy)
        {
            continue;
        }
        
        auto task = std::move(m_taskPool.front());
        m_taskPool.pop_front();
        
        lock.unlock();
        {
            task();
        }
        lock.lock();
    }
}

}
