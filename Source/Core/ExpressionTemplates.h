#pragma once

#include <cstdint>

#include "Concepts.h"
#include "Operator.h"

namespace tg
{

template <typename _Operator, typename _FirstOperand, typename _SecondOperand>
struct ExpressionTemplates final
{
/**@section Constructor */
public:
    constexpr ExpressionTemplates(const _FirstOperand& firstOperand, const _SecondOperand& secondOperand) noexcept;

/**@section Operator */
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

/**@section Method */
public:
    [[nodiscard]] constexpr const _FirstOperand& GetFirstOperand() const noexcept;
    [[nodiscard]] constexpr const _SecondOperand& GetSecondOperand() const noexcept;

/**@section Variable */
private:
    const _FirstOperand& m_firstOperand;
    const _SecondOperand& m_secondOperand;
};

template <typename _Operator, typename _FirstOperand, typename _SecondOperand>
constexpr ExpressionTemplates<_Operator, _FirstOperand, _SecondOperand>::ExpressionTemplates(const _FirstOperand& firstOperand, const _SecondOperand& secondOperand) noexcept :
    m_firstOperand(firstOperand),
    m_secondOperand(secondOperand)
{
}

template <typename _Operator, typename _FirstOperand, typename _SecondOperand>
constexpr auto ExpressionTemplates<_Operator, _FirstOperand, _SecondOperand>::operator[](int32_t index) const
{
    if constexpr (Indexable<decltype(m_firstOperand)> && Indexable<decltype(m_secondOperand)>)
    {
        return _Operator{}(m_firstOperand[index], m_secondOperand[index]);
    }
    else if constexpr (Indexable<decltype(m_firstOperand)>)
    {
        return _Operator{}(m_firstOperand[index], m_secondOperand);
    }
    else if constexpr (Indexable<decltype(m_secondOperand)>)
    {
        return _Operator{}(m_firstOperand, m_secondOperand[index]);
    }
    else
    {
        return _Operator{}(m_firstOperand, m_secondOperand);
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
    return m_firstOperand;
}

template <typename _Operator, typename _FirstOperand, typename _SecondOperand>
constexpr const _SecondOperand& ExpressionTemplates<_Operator, _FirstOperand, _SecondOperand>::GetSecondOperand() const noexcept
{
    return m_secondOperand;
}

}
