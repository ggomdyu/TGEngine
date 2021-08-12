/**
 * @file    InstanceCounter.h
 * @author  ggomdyu
 * @since   06/06/2018
 * @brief   Set of functions to manage the count of object instace.
 */

#pragma once
#include <cassert>
#include <cstdint>

namespace tgon
{

template <typename _InstanceType>
class InstanceCounter
{
/**@section Constructor */
protected:
    InstanceCounter() noexcept;

/**@section Destructor */
public:
    ~InstanceCounter() noexcept;

/**@section Method */
public:
    static int32_t GetInstanceCount() noexcept;

/**@section Variable */
protected:
    static int32_t m_instanceCount;
};

template <typename _InstanceType>
int32_t InstanceCounter<_InstanceType>::m_instanceCount = 0;

template <typename _InstanceType, int32_t _MaxInstanceCount>
class InstanceCountLimiter :
    private InstanceCounter<_InstanceType>
{
/**@section Constructor */
protected:
    InstanceCountLimiter() noexcept;
};

template <typename _InstanceType>
inline InstanceCounter<_InstanceType>::InstanceCounter() noexcept
{
    ++m_instanceCount;
}

template <typename _InstanceType>
inline InstanceCounter<_InstanceType>::~InstanceCounter() noexcept
{
    --m_instanceCount;
}

template <typename _InstanceType>
inline int32_t InstanceCounter<_InstanceType>::GetInstanceCount() noexcept
{
    return m_instanceCount;
}

template <typename _InstanceType, int32_t _MaxInstanceCount>
inline InstanceCountLimiter<_InstanceType, _MaxInstanceCount>::InstanceCountLimiter() noexcept
{
    assert(InstanceCounter<_InstanceType>::GetInstanceCount() <= _MaxInstanceCount && "The object instance count is more than specified.");
}

} /* namespace tgon */
