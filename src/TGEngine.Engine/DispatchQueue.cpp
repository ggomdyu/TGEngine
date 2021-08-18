#include "DispatchQueue.h"
#include "Thread.h"

TG_NAMESPACE_BEGIN
    
void SerialDispatchQueue::AddAsyncTask(const Delegate<void()>& task)
{
    std::lock_guard lock(_mutex);

    _taskPool.push_back(task);
}

void SerialDispatchQueue::AddAsyncTask(Delegate<void()>&& task)
{
    std::lock_guard lock(_mutex);

    _taskPool.push_back(std::move(task));
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
    for (auto& task : _taskPool)
    {
        task();
    }
    
    _taskPool.clear();
}

ConcurrentDispatchQueue::ConcurrentDispatchQueue(int32_t threadPoolCount) :
    _needToDestroy(false)
{
    _threadPool.reserve(threadPoolCount);
    for (int32_t i = 0; i < threadPoolCount; ++i)
    {
        _threadPool.emplace_back(&ConcurrentDispatchQueue::DispatchQueueHandler, this);
    }
}

ConcurrentDispatchQueue::~ConcurrentDispatchQueue()
{
    _needToDestroy = true;
    _cv.notify_all();
    
    for (auto& thread : _threadPool)
    {
        if (thread.Joinable())
        {
            thread.Join();
        }
    }
}
    
void ConcurrentDispatchQueue::AddAsyncTask(const Delegate<void()>& task)
{
    std::lock_guard lock(_mutex);

    _taskPool.push_back(task);
    _cv.notify_one();
}

void ConcurrentDispatchQueue::AddAsyncTask(Delegate<void()>&& task)
{
    std::lock_guard lock(_mutex);
    
    _taskPool.push_back(std::move(task));
    _cv.notify_one();
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
    std::unique_lock lock(_mutex);
    
    while (_needToDestroy == false)
    {
        _cv.wait(lock, [this]() {
            return (_taskPool.size() > 0) || _needToDestroy;
        });

        if ((_taskPool.size() <= 0) || _needToDestroy)
        {
            continue;
        }
        
        auto task = std::move(_taskPool.front());
        _taskPool.pop_front();
        
        lock.unlock();
        {
            task();
        }
        lock.lock();
    }
}

}
