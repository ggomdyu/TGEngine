#pragma once

namespace tg
{

class NonCopyable
{
/**@section Constructor */
public:
    NonCopyable() = default;
    NonCopyable(const NonCopyable&) = delete;
    NonCopyable(NonCopyable&&) = default;

/**@section Destructor */
public:
    ~NonCopyable() = default;
    
/**@section Operator */
public:
    NonCopyable& operator=(const NonCopyable&) noexcept = delete;
    NonCopyable& operator=(NonCopyable&&) noexcept = default;
};

}
