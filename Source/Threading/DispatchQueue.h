#pragma once

#include <deque>
#include <vector>
#include <mutex>

#include "Core/Delegate.h"
#include "Core/NonCopyable.h"

#include "Thread.h"

namespace tg
{

class DispatchQueue :
    private NonCopyable
{
/**@section Destructor */
public:
    virtual ~DispatchQueue() = default;
    
/**@section Method */
public:
    virtual void AddAsyncTask(const Delegate<void()>& task) = 0;
    virtual void AddAsyncTask(Delegate<void()>&& task) = 0;
    virtual void AddSyncTask(const Delegate<void()>& task) = 0;
    virtual void AddSyncTask(Delegate<void()>&& task) = 0;
    void Dispatch();
    
/**@section Variable */
protected:
    std::mutex m_mutex;
    std::deque<Delegate<void()>> m_taskPool;
};
    
class SerialDispatchQueue final :
    public DispatchQueue
{
/**@section Method */
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
/**@section Constructor */
public:
    explicit ConcurrentDispatchQueue(int32_t threadPoolCount);
    
/**@section Destructor */
public:
    ~ConcurrentDispatchQueue() override;

/**@section Method */
public:
    void AddAsyncTask(const Delegate<void()>& task) override;
    void AddAsyncTask(Delegate<void()>&& task) override;
    void AddSyncTask(const Delegate<void()>& task) override;
    void AddSyncTask(Delegate<void()>&& task) override;
    
private:
    void DispatchQueueHandler();

/**@section Variable */
private:
    std::condition_variable m_cv;
    std::vector<Thread> m_threadPool;
    bool m_needToDestroy;
};
    
}
