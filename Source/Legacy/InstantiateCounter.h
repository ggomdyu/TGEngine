/**
 * @file    InstantiateCounter.h
 * @author  ggomdyu
 * @since   09/24/2017
 * @brief   Set of functions to manage the count of object instantiation.
 */

#pragma once
#include <cassert>
#include <cstdint>

namespace tgon
{

template <typename _InstanceType>
class InstantiateCounter
{
/**@section Constructor */
protected:
    InstantiateCounter() noexcept;
    
/**@section Method */
public:
    static int32_t GetInstantiateCount() noexcept;

/**@section Variable */
protected:
    static int32_t m_instantiateCount;
};

template <typename _InstanceType>
int32_t InstantiateCounter<_InstanceType>::m_instantiateCount = 0;

template <typename _InstanceType, int32_t _MaxInstantiateCount>
class InstantiateCountLimiter :
    private InstantiateCounter<_InstanceType>
{
/**@section Constructor */
protected:
    InstantiateCountLimiter() noexcept;
};

template <typename _InstanceType>
inline InstantiateCounter<_InstanceType>::InstantiateCounter() noexcept
{
    ++m_instantiateCount;
}

template <typename _InstanceType>
inline int32_t InstantiateCounter<_InstanceType>::GetInstantiateCount() noexcept
{
    return m_instantiateCount;
}

template <typename _InstanceType, int32_t _MaxInstantiateCount>
inline InstantiateCountLimiter<_InstanceType, _MaxInstantiateCount>::InstantiateCountLimiter() noexcept
{
    assert(InstantiateCounter<_InstanceType>::GetInstantiateCount() <= _MaxInstantiateCount && "The object has been instantiated more than the specified count.");
}

} /* namespace tgon */
