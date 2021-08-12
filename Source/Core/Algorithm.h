#pragma once

#include <cstddef>
#include <tuple>
#include <type_traits>
#include <array>
#include <algorithm>

#include "Core/Concepts.h"

namespace tg
{

template <typename _Value>
constexpr _Value AlignOf(const _Value& value, size_t alignment)
{
    return (value + alignment - 1) & ~(alignment - 1);
}

template <typename _Char>
constexpr bool IsPalindrome(const _Char* str, size_t strLen)
{
    for (size_t i = 0; i < strLen / 2; ++i)
    {
        if (str[i] != str[strLen - i])
        {
            return false;
        }
    }
    return true;
}

template <typename _Value> requires Integral<_Value>
constexpr bool IsPrimeNumber(const _Value& value) noexcept
{
    if (value <= 1)
    {
        return false;
    }

    if (value % 2 == 0)
    {
        return (value == 2);
    }

    for (decltype(value) i = 3; i < value; ++i)
    {
        if (value % i == 0)
        {
            return false;
        }
    }

    return true;
}

template <size_t _TupleIndex = 0, typename _Callback, typename... _Types> requires (_TupleIndex == sizeof...(_Types))
void ForEach(std::tuple<_Types...>& tuple, const _Callback& callback) noexcept
{
}

template <size_t _TupleIndex = 0, typename _Callback, typename... _Types> requires (_TupleIndex < sizeof...(_Types))
void ForEach(std::tuple<_Types...>& tuple, const _Callback& callback) noexcept
{
    callback(std::get<_TupleIndex>(tuple));
    ForEach<_TupleIndex + 1, _Callback, _Types...>(tuple, callback);
}

template <typename... _Types, typename _Callback>
void ForEach(std::pair<_Types...>& pair, const _Callback& callback)
{
    callback(pair.first);
    callback(pair.second);
}

template <typename _It, typename _Callback>
void ForEach(_It beginIt, _It endIt, const _Callback& callback)
{
    while (beginIt != endIt)
    {
        callback(*beginIt);

        ++beginIt;
    }
}

template <typename _Container, typename _Callback>
void ForEach(_Container& container, const _Callback& callback)
{
    ForEach(container.begin(), container.end(), callback);
}

template <typename _Container, typename _Callback>
void ForEach(const _Container& container, const _Callback& callback)
{
    ForEach(container.cbegin(), container.cend(), callback);
}

template <typename _Container, typename _Predicate>
void EraseAll(_Container& container, const _Predicate& predicate)
{
    for (auto it = container.begin(); it != container.end();)
    {
        if (predicate(*it) == true)
        {
            it = container.erase(it);
        }
        else
        {
            ++it;
        }
    }
}

template <typename _Container, typename _Predicate, typename _Callback>
void FindAll(_Container& container, const _Predicate& predicate, const _Callback& callback)
{
    for (auto it = container.begin(); it != container.end(); ++it)
    {
        if (predicate(*it) == true)
        {
            callback(*it);
        }
    }
}

template <typename _ArrayElem, size_t _ArraySize>
constexpr size_t GetArraySize(const _ArrayElem(&)[_ArraySize]) noexcept
{
    return _ArraySize;
}

template <typename _ArrayElem, size_t _ArraySize>
constexpr size_t GetArraySize(const std::array<_ArrayElem, _ArraySize>&) noexcept
{
    return _ArraySize;
}

template <typename... _Types>
constexpr std::array<std::decay_t<std::common_type_t<_Types...>>, sizeof...(_Types)> MakeArray(_Types&&... args) noexcept
{
    return {std::forward<_Types>(args)...};
}

template <typename _Enum> requires std::is_enum_v<_Enum>
constexpr std::underlying_type_t<_Enum> UnderlyingCast(_Enum value) noexcept
{
    return static_cast<std::underlying_type_t<_Enum>>(value);
}

template <typename _Type>
void Swap(_Type& lhs, _Type& rhs) noexcept(std::is_nothrow_move_constructible<_Type>::value && std::is_nothrow_move_assignable<_Type>::value)
{
    _Type temp = std::move(lhs);
    lhs = std::move(rhs);
    rhs = std::move(temp);
}

}
