#pragma once

#include <cstdint>

#include "Operator.h"

TG_NAMESPACE_BEGIN

template <typename _Operator, typename _FirstOperand, typename _SecondOperand>
struct ExpressionTemplates final
{
public:
    constexpr ExpressionTemplates(const _FirstOperand& firstOperand, const _SecondOperand& secondOperand) noexcept;

public:
    constexpr auto operator[](int32_t index) const;
    template <typename _SecondOperand2>
    constexpr ExpressionTemplates<Add, ExpressionTemplates, _SecondOperand2> operator+(const _SecondOperand2& rhs) const noexcept;
    template <typename _SecondOperand2>
    constexpr ExpressionTemplates<Subtract, ExpressionTemplates, _SecondOperand2> operator-(const _SecondOperand2& rhs) const noexcept;
    template <typename _SecondOperand2>
    constexpr ExpressionTemplates<Multiply, ExpressionTemplates, _SecondOperand2> operator*(const _SecondOperand2& rhs) const noexcept;
    template <typename _SecondOperand2>
    constexpr ExpressionTemplates<Divide, ExpressionTemplates, _SecondOperand2> operator/(const _SecondOperand2& rhs) const noexcept;

public:
    constexpr const _FirstOperand& GetFirstOperand() const noexcept;
    constexpr const _SecondOperand& GetSecondOperand() const noexcept;

private:
    const _FirstOperand& _firstOperand;
    const _SecondOperand& _secondOperand;
};

template <typename _Operator, typename _FirstOperand, typename _SecondOperand>
constexpr ExpressionTemplates<_Operator, _FirstOperand, _SecondOperand>::ExpressionTemplates(const _FirstOperand& firstOperand, const _SecondOperand& secondOperand) noexcept :
    _firstOperand(firstOperand),
    _secondOperand(secondOperand)
{
}

template <typename _Operator, typename _FirstOperand, typename _SecondOperand>
constexpr auto ExpressionTemplates<_Operator, _FirstOperand, _SecondOperand>::operator[](int32_t index) const
{
    if constexpr (Indexable<decltype(_firstOperand)> && Indexable<decltype(_secondOperand)>)
    {
        return _Operator{}(_firstOperand[index], _secondOperand[index]);
    }
    else if constexpr (Indexable<decltype(_firstOperand)>)
    {
        return _Operator{}(_firstOperand[index], _secondOperand);
    }
    else if constexpr (Indexable<decltype(_secondOperand)>)
    {
        return _Operator{}(_firstOperand, _secondOperand[index]);
    }
    else
    {
        return _Operator{}(_firstOperand, _secondOperand);
    }
}

template <typename _Operator, typename _FirstOperand, typename _SecondOperand>
template <typename _SecondOperand2>
constexpr ExpressionTemplates<Add, ExpressionTemplates<_Operator, _FirstOperand, _SecondOperand>, _SecondOperand2> ExpressionTemplates<_Operator, _FirstOperand, _SecondOperand>::operator+(const _SecondOperand2& rhs) const noexcept
{
    return {*this, rhs};
}

template <typename _Operator, typename _FirstOperand, typename _SecondOperand>
template <typename _SecondOperand2>
constexpr ExpressionTemplates<Subtract, ExpressionTemplates<_Operator, _FirstOperand, _SecondOperand>, _SecondOperand2> ExpressionTemplates<_Operator, _FirstOperand, _SecondOperand>::operator-(const _SecondOperand2& rhs) const noexcept
{
    return {*this, rhs};
}

template <typename _Operator, typename _FirstOperand, typename _SecondOperand>
template <typename _SecondOperand2>
constexpr ExpressionTemplates<Multiply, ExpressionTemplates<_Operator, _FirstOperand, _SecondOperand>, _SecondOperand2> ExpressionTemplates<_Operator, _FirstOperand, _SecondOperand>::operator*(const _SecondOperand2& rhs) const noexcept
{
    return {*this, rhs};
}

template <typename _Operator, typename _FirstOperand, typename _SecondOperand>
template <typename _SecondOperand2>
constexpr ExpressionTemplates<Divide, ExpressionTemplates<_Operator, _FirstOperand, _SecondOperand>, _SecondOperand2> ExpressionTemplates<_Operator, _FirstOperand, _SecondOperand>::operator/(const _SecondOperand2& rhs) const noexcept
{
    return {*this, rhs};
}

template <typename _Operator, typename _FirstOperand, typename _SecondOperand>
constexpr const _FirstOperand& ExpressionTemplates<_Operator, _FirstOperand, _SecondOperand>::GetFirstOperand() const noexcept
{
    return _firstOperand;
}

template <typename _Operator, typename _FirstOperand, typename _SecondOperand>
constexpr const _SecondOperand& ExpressionTemplates<_Operator, _FirstOperand, _SecondOperand>::GetSecondOperand() const noexcept
{
    return _secondOperand;
}

}
