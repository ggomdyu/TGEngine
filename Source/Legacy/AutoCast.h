#pragma once

#include <utility>

namespace tgon
{

template <typename _CastFromType, typename _CastPolicyType>
struct AutoCaster final :
    private _CastPolicyType
{
/**@section Constructor */
public:
    template <typename _CastFromType2>
    constexpr explicit AutoCaster(_CastFromType2&& castFromValue) noexcept :
        m_castFromValue(std::forward<_CastFromType2>(castFromValue))
    {
    }

/**@section Operator */
public:
    template <typename _CastToType>
    constexpr operator _CastToType() const noexcept
    {
        return _CastPolicyType::template Cast<_CastFromType, _CastToType>(m_castFromValue);
    }

/**@section Variable */
private:
    _CastFromType m_castFromValue;
};

struct SafeCastPolicy
{
/**@section Method */
protected:
    template <typename _CastFromType, typename _CastToType>
    constexpr _CastToType Cast(const _CastFromType& castFromValue) const noexcept
    {
        return static_cast<_CastToType>(castFromValue);
    }
};

struct ForceCastPolicy
{
/**@section Method */
protected:
    template <typename _CastFromType, typename _CastToType>
    constexpr _CastToType Cast(const _CastFromType& castFromValue) const noexcept
    {
        return reinterpret_cast<_CastToType>(castFromValue);
    }
};

template <typename _CastFromType>
using StaticAutoCaster = AutoCaster<_CastFromType, SafeCastPolicy>;

template <typename _CastFromType>
using ForceAutoCaster = AutoCaster<_CastFromType, ForceCastPolicy>;

template <typename _CastFromType>
constexpr StaticAutoCaster<_CastFromType> AutoCast(_CastFromType&& value) noexcept
{
	return StaticAutoCaster<_CastFromType>(std::forward<_CastFromType>(value));
}

template <typename _CastFromType>
constexpr ForceAutoCaster<_CastFromType> ForceAutoCast(_CastFromType&& value) noexcept
{
	return ForceAutoCaster<_CastFromType>(std::forward<_CastFromType>(value));
}

} /* namespace tgon */
