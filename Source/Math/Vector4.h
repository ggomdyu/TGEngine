#pragma once

#include "Matrix4x4.h"
#include "Vector3.h"

namespace tg
{

template <typename _Value> requires Arithmetic<_Value>
struct BasicVector4
{
/**@section Type */
public:
    using ValueType = _Value;

/**@section Constructor */
public:
    constexpr BasicVector4() noexcept = default;
    constexpr BasicVector4(_Value scalar) noexcept;
    constexpr BasicVector4(_Value x, _Value y, _Value z, _Value w) noexcept;
    template <typename _Operator, typename _FirstOperand, typename _SecondOperand>
    constexpr BasicVector4(const ExpressionTemplates<_Operator, _FirstOperand, _SecondOperand>& expression);

/**@section Operator */
public:
    constexpr ExpressionTemplates<Add, BasicVector4, BasicVector4> operator+(const BasicVector4& rhs) const noexcept;
    constexpr ExpressionTemplates<Subtract, BasicVector4, BasicVector4> operator-(const BasicVector4& rhs) const noexcept;
    constexpr ExpressionTemplates<Multiply, BasicVector4, _Value> operator*(const _Value& rhs) const noexcept;
    constexpr ExpressionTemplates<Divide, BasicVector4, _Value> operator/(const _Value& rhs) const;
    constexpr BasicVector4 operator*(const Matrix4x4& rhs) const noexcept;
    constexpr BasicVector4 operator-() const noexcept;
    BasicVector4& operator+=(const BasicVector4& rhs) noexcept;
    BasicVector4& operator-=(const BasicVector4& rhs) noexcept;
    BasicVector4& operator*=(const BasicVector4& rhs) noexcept;
    BasicVector4& operator*=(_Value rhs) noexcept;
    BasicVector4& operator*=(const Matrix4x4& rhs) noexcept;
    BasicVector4& operator/=(_Value rhs);
    _Value& operator[](int32_t index) noexcept;
    _Value operator[](int32_t index) const noexcept;
    constexpr bool operator==(const BasicVector4& rhs) const noexcept;
    constexpr bool operator!=(const BasicVector4& rhs) const noexcept;
    constexpr operator Vector3() const noexcept;
    
/**@section Method */
public:
    [[nodiscard]] static constexpr _Value Dot(const BasicVector4& v1, const BasicVector4& v2) noexcept;
    [[nodiscard]] constexpr _Value Dot(const BasicVector4& v) const noexcept;
    [[nodiscard]] _Value& At(int32_t index);
    [[nodiscard]] _Value At(int32_t index) const;
    [[nodiscard]] static _Value Distance(const BasicVector4& v1, const BasicVector4& v2) noexcept;
    [[nodiscard]] _Value Distance(const BasicVector4& v) const noexcept;
    [[nodiscard]] _Value Length() const noexcept;
    [[nodiscard]] _Value LengthSq() const noexcept;
    void Normalize();
    [[nodiscard]] BasicVector4 Normalized() const;
    int32_t ToString(const std::span<char8_t>& destStr) const;
    int32_t ToString(char8_t* destStr, size_t destStrBufferLen) const;
    [[nodiscard]] std::u8string ToString() const;

/**@section Variable */
public:
    _Value x{}, y{}, z{}, w{};
};
    
using Vector4 = BasicVector4<float>;
using DoubleVector4 = BasicVector4<double>;
using IntVector4 = BasicVector4<int32_t>;
using Int64Vector4 = BasicVector4<int64_t>;

template <typename... _Types> requires Arithmetic<_Types...>
BasicVector4(_Types...) -> BasicVector4<std::common_type_t<_Types...>>;

template <typename _Value> requires Arithmetic<_Value>
    constexpr BasicVector4<_Value>::BasicVector4(_Value scalar) noexcept :
    BasicVector4(scalar, scalar, scalar, scalar)
{
}

template <typename _Value> requires Arithmetic<_Value>
constexpr BasicVector4<_Value>::BasicVector4(_Value x, _Value y, _Value z, _Value w) noexcept :
    x(x),
    y(y),
    z(z),
    w(w)
{
}

template <typename _Value> requires Arithmetic<_Value>
template <typename _Operator, typename _FirstOperand, typename _SecondOperand>
constexpr BasicVector4<_Value>::BasicVector4(const ExpressionTemplates<_Operator, _FirstOperand, _SecondOperand>& expression) :
    BasicVector4(expression[0], expression[1], expression[2], expression[3])
{
}

template <typename _Value> requires Arithmetic<_Value>
constexpr ExpressionTemplates<Add, BasicVector4<_Value>, BasicVector4<_Value>> BasicVector4<_Value>::operator+(const BasicVector4& rhs) const noexcept
{
    return {*this, rhs};
}

template <typename _Value> requires Arithmetic<_Value>
constexpr ExpressionTemplates<Subtract, BasicVector4<_Value>, BasicVector4<_Value>> BasicVector4<_Value>::operator-(const BasicVector4& rhs) const noexcept
{
    return {*this, rhs};
}

template <typename _Value> requires Arithmetic<_Value>
constexpr ExpressionTemplates<Multiply, BasicVector4<_Value>, _Value> BasicVector4<_Value>::operator*(const _Value& rhs) const noexcept
{
    return {*this, rhs};
}

template <typename _Value> requires Arithmetic<_Value>
constexpr ExpressionTemplates<Divide, BasicVector4<_Value>, _Value> BasicVector4<_Value>::operator/(const _Value& rhs) const
{
    return {*this, rhs};
}

template <typename _Value> requires Arithmetic<_Value>
constexpr ExpressionTemplates<Multiply, BasicVector4<_Value>, _Value> operator*(const _Value& lhs, const BasicVector4<_Value>& rhs) noexcept
{
    return {rhs, lhs};
}

template <typename _Value> requires Arithmetic<_Value>
constexpr BasicVector4<_Value> BasicVector4<_Value>::operator*(const Matrix4x4& rhs) const noexcept
{
    return Vector4(
        (x * rhs.m00) + (y * rhs.m10) + (z * rhs.m20) + (w * rhs.m30),
        (x * rhs.m01) + (y * rhs.m11) + (z * rhs.m21) + (w * rhs.m31),
        (x * rhs.m02) + (y * rhs.m12) + (z * rhs.m22) + (w * rhs.m32),
        (x * rhs.m03) + (y * rhs.m13) + (z * rhs.m23) + (w * rhs.m33)
   );
}

template <typename _Value> requires Arithmetic<_Value>
constexpr BasicVector4<_Value> BasicVector4<_Value>::operator-() const noexcept
{
    return BasicVector4(-x, -y, -z, -w);
}

template <typename _Value> requires Arithmetic<_Value>
BasicVector4<_Value>& BasicVector4<_Value>::operator+=(const BasicVector4& rhs) noexcept
{
    x += rhs.x;
    y += rhs.y;
    z += rhs.z;
    w += rhs.w;
    
    return *this;
}

template <typename _Value> requires Arithmetic<_Value>
BasicVector4<_Value>& BasicVector4<_Value>::operator-=(const BasicVector4& rhs) noexcept
{
    x -= rhs.x;
    y -= rhs.y;
    z -= rhs.z;
    w -= rhs.w;
    
    return *this;
}

template <typename _Value> requires Arithmetic<_Value>
BasicVector4<_Value>& BasicVector4<_Value>::operator*=(const BasicVector4& rhs) noexcept
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    w *= rhs.w;
    
    return *this;
}

template <typename _Value> requires Arithmetic<_Value>
BasicVector4<_Value>& BasicVector4<_Value>::operator*=(_Value rhs) noexcept
{
    x *= rhs;
    y *= rhs;
    z *= rhs;
    w *= rhs;
    
    return *this;
}
    
template <typename _Value> requires Arithmetic<_Value>
BasicVector4<_Value>& BasicVector4<_Value>::operator*=(const Matrix4x4& rhs) noexcept
{
    *this = Vector4(
        (x * rhs.m00) + (y * rhs.m10) + (z * rhs.m20) + (w * rhs.m30),
        (x * rhs.m01) + (y * rhs.m11) + (z * rhs.m21) + (w * rhs.m31),
        (x * rhs.m02) + (y * rhs.m12) + (z * rhs.m22) + (w * rhs.m32),
        (x * rhs.m03) + (y * rhs.m13) + (z * rhs.m23) + (w * rhs.m33)
    );
    
    return *this;
}
    
template <typename _Value> requires Arithmetic<_Value>
BasicVector4<_Value>& BasicVector4<_Value>::operator/=(_Value rhs)
{
    x /= rhs;
    y /= rhs;
    z /= rhs;
    w /= rhs;
    
    return *this;
}

template <typename _Value> requires Arithmetic<_Value>
_Value& BasicVector4<_Value>::operator[](int32_t index) noexcept
{
    return *(&x + index);
}

template <typename _Value> requires Arithmetic<_Value>
_Value BasicVector4<_Value>::operator[](int32_t index) const noexcept
{
    return *(&x + index);
}

template <typename _Value> requires Arithmetic<_Value>
constexpr bool BasicVector4<_Value>::operator==(const BasicVector4& rhs) const noexcept
{
    return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
}

template <typename _Value> requires Arithmetic<_Value>
constexpr bool BasicVector4<_Value>::operator!=(const BasicVector4& rhs) const noexcept
{
    return !(*this == rhs);
}

template <typename _Value> requires Arithmetic<_Value>
constexpr BasicVector4<_Value>::operator Vector3() const noexcept
{
    return Vector3(x, y, z);
}

template <typename _Value> requires Arithmetic<_Value>
constexpr _Value BasicVector4<_Value>::Dot(const BasicVector4& v1, const BasicVector4& v2) noexcept
{
    return v1.Dot(v2);
}
    
template <typename _Value> requires Arithmetic<_Value>
constexpr _Value BasicVector4<_Value>::Dot(const BasicVector4& v) const noexcept
{
    return (x * v.x) + (y * v.y) + (z * v.z) + (w * v.w);
}

template <typename _Value> requires Arithmetic<_Value>
_Value& BasicVector4<_Value>::At(int32_t index)
{
    assert(index < 4 && index > -1);
    return *(&x + index);
}

template <typename _Value> requires Arithmetic<_Value>
_Value BasicVector4<_Value>::At(int32_t index) const
{
    assert(index < 4 && index > -1);
    return *(&x + index);
}

template <typename _Value> requires Arithmetic<_Value>
_Value BasicVector4<_Value>::Distance(const BasicVector4& v1, const BasicVector4& v2) noexcept
{
    return BasicVector4(v1 - v2).Length();
}

template <typename _Value> requires Arithmetic<_Value>
_Value BasicVector4<_Value>::Distance(const BasicVector4& v) const noexcept
{
    return BasicVector4(*this - v).Length();
}

template <typename _Value> requires Arithmetic<_Value>
_Value BasicVector4<_Value>::Length() const noexcept
{
    return std::sqrt(this->LengthSq());
}

template <typename _Value> requires Arithmetic<_Value>
_Value BasicVector4<_Value>::LengthSq() const noexcept
{
    return (x * x) + (y * y) + (z * z) + (w * w);
}

template <typename _Value> requires Arithmetic<_Value>
void BasicVector4<_Value>::Normalize()
{
    _Value length = this->Length();
    
    x = x / length;
    y = y / length;
    z = z / length;
    w = w / length;
}

template <typename _Value> requires Arithmetic<_Value>
BasicVector4<_Value> BasicVector4<_Value>::Normalized() const
{
    _Value length = this->Length();
    
    return BasicVector4(x / length, y / length, z / length, w / length);
}

template <typename _Value> requires Arithmetic<_Value>
int32_t BasicVector4<_Value>::ToString(const std::span<char8_t>& destStr) const
{
    return this->ToString(&destStr[0], destStr.size());
}

template <typename _Value> requires Arithmetic<_Value>
std::u8string BasicVector4<_Value>::ToString() const
{
    std::array<char8_t, 1024> str{};
    const int32_t strLen = this->ToString(str);

    return {&str[0], static_cast<size_t>(strLen)};
}

template <typename _Value> requires Arithmetic<_Value>
int32_t BasicVector4<_Value>::ToString(char8_t* destStr, size_t destStrBufferLen) const
{
    const auto destStrLen = fmt::format_to_n(destStr, sizeof(destStr[0]) * (destStrBufferLen - 1), u8"{} {} {} {}", x, y, z, w).size;
    destStr[destStrLen] = u8'\0';

    return static_cast<int32_t>(destStrLen);
}

}
