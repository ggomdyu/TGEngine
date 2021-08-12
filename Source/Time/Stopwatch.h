#pragma once

#include <chrono>

namespace tg
{

class Stopwatch final
{
/**@section Constructor */
public:
    Stopwatch() noexcept;

/**@section Method */
public:
    /**@brief   Starts measuring elapsed time. */
    void Start();
    
    /**@brief   Resets elapsed time. */
    void Reset();

    /**@brief   Stops measuring elapsed time. */
    void Stop() noexcept;

    /**
     * @brief   Gets a value indicating whether the timer is running.
     * @return  True if the instance is currently running and measuring elapsed time; otherwise, false.
     */
    [[nodiscard]] bool IsRunning() const noexcept;

    /**@brief   Gets the total elapsed time measured by the current instance in seconds. */
    [[nodiscard]] int64_t GetElapsedSeconds() const;

    /**@brief   Gets the total elapsed time measured by the current instance in milliseconds. */
    [[nodiscard]] int64_t GetElapsedMilliseconds() const;

    /**@brief   Gets the total elapsed time measured by the current instance in nanoseconds. */
    [[nodiscard]] int64_t GetElapsedNanoseconds() const;

/**@section Variable */
private:
    int64_t m_oldTime;
};

inline Stopwatch::Stopwatch() noexcept :
    m_oldTime(0)
{
}

inline void Stopwatch::Start()
{
    m_oldTime = std::chrono::steady_clock::now().time_since_epoch().count();
}

inline void Stopwatch::Reset()
{
    m_oldTime = std::chrono::steady_clock::now().time_since_epoch().count();
}

inline void Stopwatch::Stop() noexcept
{
    m_oldTime = 0;
}

inline bool Stopwatch::IsRunning() const noexcept
{
    return m_oldTime > 0;
}

inline int64_t Stopwatch::GetElapsedSeconds() const
{
    return static_cast<int64_t>(this->GetElapsedMilliseconds() * 0.001);
}

inline int64_t Stopwatch::GetElapsedMilliseconds() const
{
    return static_cast<int64_t>(this->GetElapsedNanoseconds() * 0.000001);
}

inline int64_t Stopwatch::GetElapsedNanoseconds() const
{
    if (this->IsRunning())
    {
        return std::chrono::steady_clock::now().time_since_epoch().count() - m_oldTime;
    }
    else
    {
        return 0;
    }
}

}
