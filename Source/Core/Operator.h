#pragma once

namespace tg
{

struct Add
{
    template <typename _FirstOperand, typename _SecondOperand>
    constexpr auto operator()(const _FirstOperand& lhs, const _SecondOperand& rhs) const
    {
        return lhs + rhs;
    }
};

struct Subtract
{
    template <typename _FirstOperand, typename _SecondOperand>
    constexpr auto operator()(const _FirstOperand& lhs, const _SecondOperand& rhs) const
    {
        return lhs - rhs;
    }
};

struct Multiply
{
    template <typename _FirstOperand, typename _SecondOperand>
    constexpr auto operator()(const _FirstOperand& lhs, const _SecondOperand& rhs) const
    {
        return lhs * rhs;
    }
};

struct Divide
{
    template <typename _FirstOperand, typename _SecondOperand>
    constexpr auto operator()(const _FirstOperand& lhs, const _SecondOperand& rhs) const
    {
        return lhs / rhs;
    }
};

}