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

TG_NAMESPACE_BEGIN

template <typename>
struct FunctionTraits;

template <typename _Return, typename... Ts>
struct FunctionTraits<_Return(Ts...)>
{
public:
    using ReturnType = _Return;
    using FunctionType = _Return(Ts...);
    
public:
    static constexpr bool IsMemberFunction = false;
    static constexpr bool IsFunctor = false;
    static constexpr size_t ArgumentCount = sizeof...(Ts);
};

template <typename _Return, typename... Ts>
struct FunctionTraits<_Return(*)(Ts...)> : FunctionTraits<_Return(Ts...)> {};

template <typename _Return, typename _Class, typename... Ts>
struct FunctionTraits<_Return(_Class::*)(Ts...)> :
    FunctionTraits<std::remove_cv_t<_Return(Ts...)>>
{
public:
    using ClassType = _Class;
    
public:
    static constexpr bool IsMemberFunction = true;
};

template <typename _Return, typename _Class, typename... Ts>
struct FunctionTraits<_Return(_Class::*)(Ts...) const> : FunctionTraits<_Return(_Class::*)(Ts...)> {};
    
template <typename _Return, typename _Class, typename... Ts>
struct FunctionTraits<_Return(_Class::*)(Ts...) volatile> : FunctionTraits<_Return(_Class::*)(Ts...)> {};

template <typename _Return, typename _Class, typename... Ts>
struct FunctionTraits<_Return(_Class::*)(Ts...) const volatile> : FunctionTraits<_Return(_Class::*)(Ts...)> {};

template <typename _Function>
struct FunctionTraits :
    FunctionTraits<decltype(&_Function::operator())>
{
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

template <typename _Type, typename... Ts>
constexpr bool IsAllSame = std::bool_constant<(std::is_same_v<_Type, Ts> && ...)>::value;

template <typename _Type, typename... Ts>
constexpr bool IsAnyOf = std::bool_constant<(std::is_same_v<_Type, Ts> || ...)>::value;

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
