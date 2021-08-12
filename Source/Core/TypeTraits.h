#pragma once

#include <type_traits>

namespace tg::detail
{

template <typename _Type>
struct RemoveAllPointers
{
    using Type = _Type;
};

template <typename _Type>
struct RemoveAllPointers<_Type*>
{
    using Type = typename RemoveAllPointers<_Type>::Type;
};

}

namespace tg
{

template <typename>
struct FunctionTraits;

template <typename _Return, typename... _Types>
struct FunctionTraits<_Return(_Types...)>
{
/**@section Type */
public:
    using ReturnType = _Return;
    using FunctionType = _Return(_Types...);
    
/**@section Variable */
public:
    static constexpr bool IsMemberFunction = false;
    static constexpr bool IsFunctor = false;
    static constexpr size_t ArgumentCount = sizeof...(_Types);
};

template <typename _Return, typename... _Types>
struct FunctionTraits<_Return(*)(_Types...)> : FunctionTraits<_Return(_Types...)> {};

template <typename _Return, typename _Class, typename... _Types>
struct FunctionTraits<_Return(_Class::*)(_Types...)> :
    FunctionTraits<std::remove_cv_t<_Return(_Types...)>>
{
/**@section Type */
public:
    using ClassType = _Class;
    
/**@section Variable */
public:
    static constexpr bool IsMemberFunction = true;
};

template <typename _Return, typename _Class, typename... _Types>
struct FunctionTraits<_Return(_Class::*)(_Types...) const> : FunctionTraits<_Return(_Class::*)(_Types...)> {};
    
template <typename _Return, typename _Class, typename... _Types>
struct FunctionTraits<_Return(_Class::*)(_Types...) volatile> : FunctionTraits<_Return(_Class::*)(_Types...)> {};

template <typename _Return, typename _Class, typename... _Types>
struct FunctionTraits<_Return(_Class::*)(_Types...) const volatile> : FunctionTraits<_Return(_Class::*)(_Types...)> {};

template <typename _Function>
struct FunctionTraits :
    FunctionTraits<decltype(&_Function::operator())>
{
/**@section Variable */
public:
    static constexpr bool IsFunctor = true;
    static constexpr bool IsMemberFunction = false;
};

template <typename _Type>
using RemoveAllPointers = typename detail::RemoveAllPointers<_Type>::Type;

template <typename _Type>
using RawType = std::remove_cv_t<RemoveAllPointers<std::decay_t<_Type>>>;

template <typename _Type>
constexpr bool IsRawType = std::is_same_v<RawType<_Type>, _Type>;

template <typename _Type, typename... _Types>
constexpr bool IsAllSame = std::bool_constant<(std::is_same_v<_Type, _Types> && ...)>::value;

template <typename _Type, typename... _Types>
constexpr bool IsAnyOf = std::bool_constant<(std::is_same_v<_Type, _Types> || ...)>::value;

template <typename _Type>
constexpr bool IsChar = IsAnyOf<_Type, char, char8_t, char16_t, char32_t, wchar_t>;

template <typename _Type, typename = std::void_t<>>
constexpr bool IsHashable = false;

template <typename _Type>
constexpr bool IsHashable<_Type, std::void_t<decltype(std::hash<_Type>().operator()(std::declval<_Type>()))>> = true;

template <typename _Type, typename = std::void_t<>>
constexpr bool IsIndexable = false;

template <typename _Type>
constexpr bool IsIndexable<_Type, std::void_t<decltype(std::declval<_Type>().operator[](0))>> = true;

}
