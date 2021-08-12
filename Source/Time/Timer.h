#pragma once

#include <cstdint>

#include "Core/DelegateChain.h"

namespace tg
{
    
class Timer final
{
/**@section Constructor */
public:
    explicit Timer(bool isAutoReset) noexcept;
    Timer() noexcept;
    Timer(int64_t interval, bool isAutoReset) noexcept;

/**@section Method */
public:
    void Update();
    
    /**
     * @brief   Starts this timer.
     * @remarks If Start is called and AutoReset is set to false, the Timer raises the OnTimeElapsed event only once,
     *          the first time the interval elapses. If Start is called and AutoReset is true, the Timer raises
     *          the OnTimeElapsed event the first time the interval elapses and continues to raise the event on the
     *          specified interval.
     */
    void Start();
    
    /**
     * @brief   Stops raising the OnTimeElapsed event by setting m_isEnabled to false.
     */
    void Stop();
    
    /**
     * @brief   Sets a bool indicating whether the Timer should raise the event only once or repeatedly.
     */
    void SetAutoReset(bool isAutoReset);
    
    /**
     * @brief   Gets a bool indicating whether the Timer should raise the event only once or repeatedly.
     */
    [[nodiscard]] bool IsAutoReset() const noexcept;
    
    /**
     * @brief   Sets the interval, expressed in milliseconds, at which to raise the OnTimeElapsed event.
     */
    void SetInterval(int64_t interval);
    
    /**
     * @brief   Gets the interval, expressed in milliseconds, at which to raise the OnTimeElapsed event.
     */
    [[nodiscard]] int64_t GetInterval() const noexcept;
    
/**@section Variable */
public:
    /**
     * @brief   Occurs when the interval elapses.
     */
    DelegateChain<void()> OnTimeElapsed;
    
/**@section Variable */
private:
    int64_t m_interval;
    int64_t m_prevTime;
    bool m_isAutoReset;
    bool m_isEnabled;
};

}
