#pragma once

#include <type_traits>

#include "Rtti.h"

namespace tg
{

template <typename _CastTo, typename _CastFrom, std::enable_if_t<std::is_convertible_v<_CastFrom, _CastTo>>* = nullptr>
_CastTo DynamicCast(_CastFrom&& ptr) noexcept
{
    return ptr;
}

template <typename _CastTo, typename _CastFrom, std::enable_if_t<!std::is_convertible_v<_CastFrom, _CastTo>>* = nullptr>
_CastTo DynamicCast(_CastFrom&& ptr) noexcept
{
    const Rtti* rtti = ptr->GetRTTI();
    while (rtti != nullptr)
    {
        if (rtti == GetRtti<_CastTo>())
        {
            return reinterpret_cast<_CastTo>(std::forward<_CastFrom>(ptr));
        }
        else
        {
            rtti = rtti->GetSuperRtti();
        }
    }
    
    return nullptr;
}

}
