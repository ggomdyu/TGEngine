#pragma once

#include <vector>
#include <type_traits>

#include "Core/Delegate.h"

namespace tg
{

template <typename _WeightType>
class WeightSelector
{
    static_assert(std::is_scalar_v<_WeightType>, "_WeightType must be scalar type.");

/**@section Type */
public:
    using WeightType = _WeightType;
    using WeightHandlerType = Delegate<void()>;

/**@section Public struct */
public:
    struct WeightDesc
    {
        WeightType weight;
        WeightHandlerType weightHandler;
    };

/**@section Constructor */
public:
    WeightSelector() noexcept;

/**@section Method */
public:
    void Add(WeightType weight, const WeightHandlerType& handler);
    void Add(WeightType weight, WeightHandlerType&& handler);
    void Erase(const WeightHandlerType& handler);
    void Select();
    void Clear();
    bool IsEmpty() const noexcept;

/**@section Variable */
private:
    std::vector<WeightDesc> m_weightDescList;

    WeightType m_totalWeight;
};

template <typename _WeightType>
inline WeightSelector<_WeightType>::WeightSelector() noexcept :
    m_totalWeight(0)
{
}

template <typename _WeightType>
inline void WeightSelector<_WeightType>::Add(WeightType weight, const WeightHandlerType& handler)
{
    assert(weight >= 0 && "weight cannot be negative.");

    m_totalWeight += weight;
    m_weightDescList.push_back(WeightDesc{weight, handler});
}

template <typename _WeightType>
inline void WeightSelector<_WeightType>::Add(WeightType weight, WeightHandlerType&& handler)
{
    assert(weight >= 0 && "weight cannot be zero or negative.");

    m_totalWeight += weight;
    m_weightDescList.push_back(WeightDesc{weight, handler});
}

template <typename _WeightType>
inline void WeightSelector<_WeightType>::Erase(const WeightHandlerType& handler)
{
    auto it = std::find_if(m_weightDescList.begin(), m_weightDescList.end(), [&](const WeightDesc& comp)
    {
        return handler == comp.weightHandler;
    });

    if (it != m_weightDescList.end())
    {
        m_totalWeight -= it->weight;
        m_weightDescList.erase(it);
    }
}

template <typename _WeightType>
inline void WeightSelector<_WeightType>::Select()
{
    auto rand = Random().Next(WeightType(0), m_totalWeight);
    
    WeightType accumulatedWeight = 0;
    for (auto& weightDesc : m_weightDescList)
    {
        if (weightDesc.weight == WeightType(0))
        {
            continue;
        }

        accumulatedWeight += weightDesc.weight;

        if (accumulatedWeight >= rand)
        {
            if (weightDesc.weightHandler != nullptr)
            {
                weightDesc.weightHandler();
            }
            break;
        }
    }
}

template <typename _WeightType>
inline void WeightSelector<_WeightType>::Clear()
{
    m_totalWeight = 0;
    m_weightDescList.clear();
}

template <typename _WeightType>
inline bool WeightSelector<_WeightType>::IsEmpty() const noexcept
{
    return m_weightDescList.size() <= 0;
}

}
