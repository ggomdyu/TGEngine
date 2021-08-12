#pragma once

#include "Vector2.h"

namespace tg
{

template <typename _Value> requires Arithmetic<_Value>
struct BasicVector3
{
/**@section Type */
public:
    using ValueType = _Value;

/**@section Constructor */
public:
    constexpr BasicVector3() noexcept = default;
    constexpr BasicVector3(_Value scalar) noexcept;
    constexpr BasicVector3(_Value x, _Value y) noexcept;
    constexpr BasicVector3(_Value x, _Value y, _Value z) noexcept;
    template <typename _Operator, typename _FirstOperand, typename _SecondOperand>
    constexpr BasicVector3(const ExpressionTemplates<_Operator, _FirstOperand, _SecondOperand>& expression);
    
/**@section Operator */
public:
    constexpr ExpressionTemplates<Add, BasicVector3, BasicVector3> operator+(const BasicVector3& rhs) const noexcept;
    constexpr ExpressionTemplates<Subtract, BasicVector3, BasicVector3> operator-(const BasicVector3& rhs) const noexcept;
    constexpr ExpressionTemplates<Multiply, BasicVector3, _Value> operator*(const _Value& rhs) const noexcept;
    constexpr ExpressionTemplates<Divide, BasicVector3, _Value> operator/(const _Value& rhs) const;
    constexpr BasicVector3 operator-() const noexcept;
    BasicVector3& operator+=(const BasicVector3& rhs) noexcept;
    BasicVector3& operator-=(const BasicVector3& rhs) noexcept;
    BasicVector3& operator*=(const BasicVector3& rhs) noexcept;
    BasicVector3& operator*=(_Value rhs) noexcept;
    BasicVector3& operator/=(_Value rhs);
    _Value& operator[](int32_t index) noexcept;
    _Value operator[](int32_t index) const noexcept;
    constexpr bool operator==(const BasicVector3& rhs) const noexcept;
    constexpr bool operator!=(const BasicVector3& rhs) const noexcept;
    constexpr operator Vector2() const noexcept;
    
/**@section Method */
public:
    [[nodiscard]] static constexpr BasicVector3 Reflect(const BasicVector3& inDirection, const BasicVector3& inPlaneNormal) noexcept;
    [[nodiscard]] static constexpr _Value Dot(const BasicVector3& v1, const BasicVector3& v2) noexcept;
    [[nodiscard]] constexpr _Value Dot(const BasicVector3& v) const noexcept;
    [[nodiscard]] static constexpr BasicVector3 Cross(const BasicVector3& v1, const BasicVector3& v2) noexcept;
    [[nodiscard]] _Value& At(int32_t index);
    [[nodiscard]] _Value At(int32_t index) const;
    //[[nodiscard]] static float Angle(const BasicVector3& v1, const BasicVector3& v2) noexcept;
    [[nodiscard]] static _Value Distance(const BasicVector3& v1, const BasicVector3& v2);
    [[nodiscard]] _Value Distance(const BasicVector3& v) const noexcept;
    [[nodiscard]] _Value Length() const;
    [[nodiscard]] _Value LengthSq() const noexcept;
    void Normalize();
    [[nodiscard]] BasicVector3 Normalized() const;
    int32_t ToString(const std::span<char8_t>& destStr) const;
    int32_t ToString(char8_t* destStr, size_t destStrBufferLen) const;
    [[nodiscard]] std::u8string ToString() const;
    
/**@section Variable */
public:
    _Value x{}, y{}, z{};
};
    
using Vector3 = BasicVector3<float>;
using DoubleVector3 = BasicVector3<double>;
using IntVector3 = BasicVector3<int32_t>;
using Int64Vector3 = BasicVector3<int64_t>;

template <typename... _Types> requires Arithmetic<_Types...>
BasicVector3(_Types...) -> BasicVector3<std::common_type_t<_Types...>>;

template <typename _Value> requires Arithmetic<_Value>
constexpr BasicVector3<_Value>::BasicVector3(_Value scalar) noexcept :
    BasicVector3(scalar, scalar, scalar)
{
}

template <typename _Value> requires Arithmetic<_Value>
constexpr BasicVector3<_Value>::BasicVector3(_Value x, _Value y) noexcept :
    BasicVector3(x, y, 0.0f)
{
}

template <typename _Value> requires Arithmetic<_Value>
constexpr BasicVector3<_Value>::BasicVector3(_Value x, _Value y, _Value z) noexcept :
    x(x),
    y(y),
    z(z)
{
}

template <typename _Value> requires Arithmetic<_Value>
template <typename _Operator, typename _FirstOperand, typename _SecondOperand>
constexpr BasicVector3<_Value>::BasicVector3(const ExpressionTemplates<_Operator, _FirstOperand, _SecondOperand>& expression) :
    BasicVector3(expression[0], expression[1], expression[2])
{
}

template <typename _Value> requires Arithmetic<_Value>
constexpr ExpressionTemplates<Add, BasicVector3<_Value>, BasicVector3<_Value>> BasicVector3<_Value>::operator+(const BasicVector3& rhs) const noexcept
{
    return {*this, rhs};
}

template <typename _Value> requires Arithmetic<_Value>
constexpr ExpressionTemplates<Subtract, BasicVector3<_Value>, BasicVector3<_Value>> BasicVector3<_Value>::operator-(const BasicVector3& rhs) const noexcept
{
    return {*this, rhs};
}

template <typename _Value> requires Arithmetic<_Value>
constexpr ExpressionTemplates<Multiply, BasicVector3<_Value>, _Value> BasicVector3<_Value>::operator*(const _Value& rhs) const noexcept
{
    return {*this, rhs};
}

template <typename _Value> requires Arithmetic<_Value>
constexpr ExpressionTemplates<Divide, BasicVector3<_Value>, _Value> BasicVector3<_Value>::operator/(const _Value& rhs) const
{
    return {*this, rhs};
}

template <typename _Value> requires Arithmetic<_Value>
constexpr ExpressionTemplates<Multiply, BasicVector3<_Value>, _Value> operator*(const _Value& lhs, const BasicVector3<_Value>& rhs) noexcept
{
    return {rhs, lhs};
}

template <typename _Value> requires Arithmetic<_Value>
constexpr BasicVector3<_Value> BasicVector3<_Value>::operator-() const noexcept
{
    return BasicVector3(-x, -y, -z);
}

template <typename _Value> requires Arithmetic<_Value>
BasicVector3<_Value>& BasicVector3<_Value>::operator+=(const BasicVector3& rhs) noexcept
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    
    return *this;
}

template <typename _Value> requires Arithmetic<_Value>
BasicVector3<_Value>& BasicVector3<_Value>::operator-=(const BasicVector3& rhs) noexcept
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    
    return *this;
}

template <typename _Value> requires Arithmetic<_Value>
BasicVector3<_Value>& BasicVector3<_Value>::operator*=(const BasicVector3& rhs) noexcept
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    
    return *this;
}

template <typename _Value> requires Arithmetic<_Value>
BasicVector3<_Value>& BasicVector3<_Value>::operator*=(_Value rhs) noexcept
{
    x *= rhs;
    y *= rhs;
    z *= rhs;
    
    return *this;
}

template <typename _Value> requires Arithmetic<_Value>
BasicVector3<_Value>& BasicVector3<_Value>::operator/=(_Value rhs)
{
    x /= rhs;
    y /= rhs;
    z /= rhs;
    
    return *this;
}

template <typename _Value> requires Arithmetic<_Value>
_Value& BasicVector3<_Value>::operator[](int32_t index) noexcept
{
    return *(&x + index);
}

template <typename _Value> requires Arithmetic<_Value>
_Value BasicVector3<_Value>::operator[](int32_t index) const noexcept
{
    return *(&x + index);
}

template <typename _Value> requires Arithmetic<_Value>
constexpr bool BasicVector3<_Value>::operator==(const BasicVector3& rhs) const noexcept
{
    return (x == rhs.x && y == rhs.y && z == rhs.z);
}

template <typename _Value> requires Arithmetic<_Value>
constexpr bool BasicVector3<_Value>::operator!=(const BasicVector3& rhs) const noexcept
{
    return !(*this == rhs);
}
    
template <typename _Value> requires Arithmetic<_Value>
constexpr BasicVector3<_Value>::operator Vector2() const noexcept
{
    return Vector2(x, y);
}

template <typename _Value> requires Arithmetic<_Value>
constexpr BasicVector3<_Value> BasicVector3<_Value>::Reflect(const BasicVector3& inDirection, const BasicVector3& inPlaneNormal) noexcept
{
    return inDirection + Dot(-inDirection, inPlaneNormal) * 2 * inPlaneNormal;
}

template <typename _Value> requires Arithmetic<_Value>
constexpr _Value BasicVector3<_Value>::Dot(const BasicVector3& v1, const BasicVector3& v2) noexcept
{
    return v1.Dot(v2);
}
    
template <typename _Value> requires Arithmetic<_Value>
constexpr _Value BasicVector3<_Value>::Dot(const BasicVector3& v) const noexcept
{
    return (x * v.x) + (y * v.y) + (z * v.z);
}

template <typename _Value> requires Arithmetic<_Value>
constexpr BasicVector3<_Value> BasicVector3<_Value>::Cross(const BasicVector3& v1, const BasicVector3& v2) noexcept
{
    return {
        (v1.y * v2.z) - (v1.z * v2.y),
        (v1.z * v2.x) - (v1.x * v2.z),
        (v1.x * v2.y) - (v1.y * v2.x)
    };
}

template <typename _Value> requires Arithmetic<_Value>
_Value& BasicVector3<_Value>::At(int32_t index)
{
    assert(index < 3 && index > -1);
    return *(&x + index);
}

template <typename _Value> requires Arithmetic<_Value>
_Value BasicVector3<_Value>::At(int32_t index) const
{
    assert(index < 3 && index > -1);
    return *(&x + index);
}

template <typename _Value> requires Arithmetic<_Value>
_Value BasicVector3<_Value>::Distance(const BasicVector3& v1, const BasicVector3& v2)
{
    return BasicVector3(v1 - v2).Length();
}
    
template <typename _Value> requires Arithmetic<_Value>
_Value BasicVector3<_Value>::Distance(const BasicVector3& v) const noexcept
{
    return BasicVector3(*this - v).Length();
}

template <typename _Value> requires Arithmetic<_Value>
_Value BasicVector3<_Value>::Length() const
{
    return std::sqrtf(this->LengthSq());
}

template <typename _Value> requires Arithmetic<_Value>
_Value BasicVector3<_Value>::LengthSq() const noexcept
{
    return (x * x) + (y * y) + (z * z);
}

template <typename _Value> requires Arithmetic<_Value>
void BasicVector3<_Value>::Normalize()
{
    _Value length = this->Length();
    
    x = x / length;
    y = y / length;
    z = z / length;
}

template <typename _Value> requires Arithmetic<_Value>
BasicVector3<_Value> BasicVector3<_Value>::Normalized() const
{
    _Value length = this->Length();
    
    return BasicVector3(x / length, y / length, z / length);
}

template <typename _Value> requires Arithmetic<_Value>
int32_t BasicVector3<_Value>::ToString(const std::span<char8_t>& destStr) const
{
    return this->ToString(&destStr[0], destStr.size());
}

template <typename _Value> requires Arithmetic<_Value>
std::u8string BasicVector3<_Value>::ToString() const
{
    std::array<char8_t, 1024> str{};
    const int32_t strLen = this->ToString(str);

    return {&str[0], static_cast<size_t>(strLen)};
}

template <typename _Value> requires Arithmetic<_Value>
int32_t BasicVector3<_Value>::ToString(char8_t* destStr, size_t destStrBufferLen) const
{
    const auto destStrLen = fmt::format_to_n(destStr, sizeof(destStr[0]) * (destStrBufferLen - 1), u8"{} {} {}", x, y, z).size;
    destStr[destStrLen] = u8'\0';

    return static_cast<int32_t>(destStrLen);
}

}
