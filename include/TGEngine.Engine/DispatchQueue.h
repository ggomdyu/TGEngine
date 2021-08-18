#pragma once

#include <deque>
#include <vector>
#include <mutex>

#include "Core/Delegate.h"
#include "Core/NonCopyable.h"

#include "Thread.h"

TG_NAMESPACE_BEGIN

class DispatchQueue :
    private NonCopyable
{
public:
    virtual ~DispatchQueue() = default;
    
public:
    virtual void AddAsyncTask(const Delegate<void()>& task) = 0;
    virtual void AddAsyncTask(Delegate<void()>&& task) = 0;
    virtual void AddSyncTask(const Delegate<void()>& task) = 0;
    virtual void AddSyncTask(Delegate<void()>&& task) = 0;
    void Dispatch();
    
protected:
    std::mutex _mutex;
    std::deque<Delegate<void()>> _taskPool;
};
    
class SerialDispatchQueue final :
    public DispatchQueue
{
public:
    void AddAsyncTask(const Delegate<void()>& task) override;
    void AddAsyncTask(Delegate<void()>&& task) override;
    void AddSyncTask(const Delegate<void()>& task) override;
    void AddSyncTask(Delegate<void()>&& task) override;
    void Dispatch();
};

class ConcurrentDispatchQueue final :
    public DispatchQueue
{
public:
    explicit ConcurrentDispatchQueue(int32_t threadPoolCount);
    ~ConcurrentDispatchQueue() override;

public:
    void AddAsyncTask(const Delegate<void()>& task) override;
    void AddAsyncTask(Delegate<void()>&& task) override;
    void AddSyncTask(const Delegate<void()>& task) override;
    void AddSyncTask(Delegate<void()>&& task) override;
    
private:
    void DispatchQueueHandler();

private:
    std::condition_variable _cv;
    std::vector<Thread> _threadPool;
    bool _needToDestroy;
};
    
}
