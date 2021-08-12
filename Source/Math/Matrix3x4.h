#pragma once

#include <cstdint>
#include <cassert>
#include <cmath>

#include "Vector3.h"

namespace tg
{

struct alignas(16) Matrix3x4
{
/**@section Constructor */
public:
    constexpr Matrix3x4() noexcept;
    constexpr Matrix3x4(float m00, float m01, float m02,
                        float m10, float m11, float m12,
                        float m20, float m21, float m22,
                        float m30, float m31, float m32) noexcept;

/**@section Operator */
public:
    Matrix3x4 operator+(const Matrix3x4&) const noexcept;
    Matrix3x4 operator-(const Matrix3x4&) const noexcept;
    Matrix3x4 operator*(const Matrix3x4&) const noexcept;
    Matrix3x4& operator+=(const Matrix3x4&) noexcept;
    Matrix3x4& operator-=(const Matrix3x4&) noexcept;
    Matrix3x4& operator*=(const Matrix3x4&) noexcept;
    constexpr bool operator==(const Matrix3x4&) const noexcept;
    constexpr bool operator!=(const Matrix3x4&) const noexcept;
    float& operator[](std::size_t index);
    float operator[](std::size_t index) const;

/**@section Method */
public:
    [[nodiscard]] static constexpr Matrix3x4 Identity() noexcept;
    [[nodiscard]] static constexpr Matrix3x4 Zero() noexcept;
    [[nodiscard]] static constexpr Matrix3x4 Translate(float x, float y, float z) noexcept;
    [[nodiscard]] static Matrix3x4 Rotate(float yaw, float pitch, float roll) noexcept;
    [[nodiscard]] static Matrix3x4 RotateX(float radian) noexcept;
    [[nodiscard]] static Matrix3x4 RotateY(float radian) noexcept;
    [[nodiscard]] static Matrix3x4 RotateZ(float radian) noexcept;
    [[nodiscard]] static constexpr  Matrix3x4 Scale(float x, float y, float z) noexcept;
    //static constexpr const Matrix4x3 Transposed(const Matrix3x4& matrix) noexcept;
    [[nodiscard]] static constexpr Matrix3x4 Inverse();
    [[nodiscard]] static Matrix3x4 LookAtLH(const Vector3& eyePt, const Vector3& lookAt, const Vector3& up) noexcept;
    [[nodiscard]] static Matrix3x4 LookAtRH(const Vector3& eyePt, const Vector3& lookAt, const Vector3& up) noexcept;
    [[nodiscard]] static Matrix3x4 PerspectiveLH(float fovy, float aspect, float nearZ, float farZ) noexcept;
    [[nodiscard]] static Matrix3x4 PerspectiveRH(float fovy, float aspect, float nearZ, float farZ) noexcept;
    [[nodiscard]] static constexpr Matrix3x4 OrthographicRH(float left, float right, float top, float bottom, float nearZ, float farZ) noexcept;
    [[nodiscard]] static constexpr Matrix3x4 Viewport(float x, float y, float width, float height, float minZ, float maxZ) noexcept;
    int32_t ToString(const std::span<char>& destStr) const;
    int32_t ToString(char* destStr, std::size_t destStrBufferLen) const;
    [[nodiscard]] std::string ToString() const;

/**@section Variable */
public:
    float m00, m01, m02,
          m10, m11, m12,
          m20, m21, m22,
          m30, m31, m32;
};

constexpr Matrix3x4::Matrix3x4() noexcept :
    m00(1.0f), m01(0.0f), m02(0.0f),
    m10(0.0f), m11(1.0f), m12(0.0f),
    m20(0.0f), m21(0.0f), m22(1.0f),
    m30(0.0f), m31(0.0f), m32(0.0f)
{
}

constexpr Matrix3x4::Matrix3x4(float m00, float m01, float m02,
                               float m10, float m11, float m12,
                               float m20, float m21, float m22,
                               float m30, float m31, float m32) noexcept :
    m00(m00), m01(m01), m02(m02),
    m10(m10), m11(m11), m12(m12),
    m20(m20), m21(m21), m22(m22),
    m30(m30), m31(m31), m32(m32)
{
}

inline Matrix3x4 Matrix3x4::operator-(const Matrix3x4& rhs) const noexcept
{
    return Matrix3x4(
        m00 - rhs.m00, m01 - rhs.m01, m02 - rhs.m02,
        m10 - rhs.m10, m11 - rhs.m11, m12 - rhs.m12,
        m20 - rhs.m20, m21 - rhs.m21, m22 - rhs.m22,
        m30 - rhs.m30, m31 - rhs.m31, m32 - rhs.m32
    );
}

inline Matrix3x4 Matrix3x4::operator*(const Matrix3x4& rhs) const noexcept
{
    return Matrix3x4(
        (m00 * rhs.m00) + (m01 * rhs.m10) + (m02 * rhs.m20),
        (m00 * rhs.m01) + (m01 * rhs.m11) + (m02 * rhs.m21),
        (m00 * rhs.m02) + (m01 * rhs.m12) + (m02 * rhs.m22),

        (m10 * rhs.m00) + (m11 * rhs.m10) + (m12 * rhs.m20),
        (m10 * rhs.m01) + (m11 * rhs.m11) + (m12 * rhs.m21),
        (m10 * rhs.m02) + (m11 * rhs.m12) + (m12 * rhs.m22),

        (m20 * rhs.m00) + (m21 * rhs.m10) + (m22 * rhs.m20),
        (m20 * rhs.m01) + (m21 * rhs.m11) + (m22 * rhs.m21),
        (m20 * rhs.m02) + (m21 * rhs.m12) + (m22 * rhs.m22),

        (m30 * rhs.m00) + (m31 * rhs.m10) + (m32 * rhs.m20),
        (m30 * rhs.m01) + (m31 * rhs.m11) + (m32 * rhs.m21),
        (m30 * rhs.m02) + (m31 * rhs.m12) + (m32 * rhs.m22)
    );
}

inline Matrix3x4& Matrix3x4::operator+=(const Matrix3x4& rhs) noexcept
{
    m00 += rhs.m00; m01 += rhs.m01; m02 += rhs.m02;
    m10 += rhs.m10; m11 += rhs.m11; m12 += rhs.m12;
    m20 += rhs.m20; m21 += rhs.m21; m22 += rhs.m22;
    m30 += rhs.m30; m31 += rhs.m31; m32 += rhs.m32;

    return *this;
}

inline Matrix3x4& Matrix3x4::operator-=(const Matrix3x4& rhs) noexcept
{
    m00 -= rhs.m00; m01 -= rhs.m01; m02 -= rhs.m02;
    m10 -= rhs.m10; m11 -= rhs.m11; m12 -= rhs.m12;
    m20 -= rhs.m20; m21 -= rhs.m21; m22 -= rhs.m22;
    m30 -= rhs.m30; m31 -= rhs.m31; m32 -= rhs.m32;

    return *this;
}

inline Matrix3x4 Matrix3x4::LookAtLH(const Vector3& eyePt, const Vector3& lookAt, const Vector3& up) noexcept
{
    Vector3 l = lookAt - eyePt;
    l.Normalize();
    
    Vector3 r = Vector3::Cross(up, l);
    r.Normalize();

    Vector3 u = Vector3::Cross(l, r);
    u.Normalize();

    return Matrix3x4(
        r.x, u.x, l.x,
        r.y, u.y, l.y,
        r.z, u.z, l.z,
        -Vector3::Dot(r, eyePt), -Vector3::Dot(u, eyePt), -Vector3::Dot(l, eyePt)
    );
}

inline Matrix3x4 Matrix3x4::LookAtRH(const Vector3& eyePt, const Vector3& lookAt, const Vector3& up) noexcept
{
    Vector3 l = lookAt - eyePt;
    l.Normalize();

    Vector3 r = Vector3::Cross(l, up);
    r.Normalize();

    Vector3 u = Vector3::Cross(r, l);
    u.Normalize();
    
    return Matrix3x4(
        r.x, u.x, -l.x,
        r.y, u.y, -l.y,
        r.z, u.z, -l.z,
        -Vector3::Dot(r, eyePt), -Vector3::Dot(u, eyePt), Vector3::Dot(l, eyePt)
    );
}

inline Matrix3x4 Matrix3x4::PerspectiveLH(float fovy, float aspect, float nearZ, float farZ) noexcept
{
    const auto scaleY = 1.0f / std::tan(fovy * 0.5f);
    const auto scaleX = scaleY / aspect;

    return Matrix3x4(
        scaleX, 0.0f,   0.0f,
        0.0f,   scaleY, 0.0f,
        0.0f,   0.0f,   farZ / (farZ - nearZ),
        0.0f,   0.0f,   -(farZ * nearZ) / (farZ - nearZ)
    );
}

inline Matrix3x4 Matrix3x4::PerspectiveRH(float fovy, float aspect, float nearZ, float farZ) noexcept
{
    const auto scaleY = 1.0f / std::tan(fovy * 0.5f);
    const auto scaleX = scaleY / aspect;

    return Matrix3x4(
        scaleX, 0.0f,   0.0f,
        0.0f,   scaleY, 0.0f,
        0.0f,   0.0f,   farZ / (nearZ - farZ),
        0.0f,   0.0f,   -(farZ * nearZ) / (farZ - nearZ)
    );
}
    
constexpr Matrix3x4 Matrix3x4::OrthographicRH(float left, float right, float top, float bottom, float nearZ, float farZ) noexcept
{
    const auto width = right - left;
    const auto height = bottom - top;
    const auto depth = farZ - nearZ;
    
    return Matrix3x4(
        2 / width,  0.0f,           0.0f,
        0.0f,       2 / height,     0.0f,
        0.0f,       0.0f,           -2.0f / depth,
        -(right + left) / width,    -(top + bottom) / height,   -(farZ + nearZ) / depth
    );
}

constexpr Matrix3x4 Matrix3x4::Viewport(float x, float y, float width, float height, float minZ, float maxZ) noexcept
{
    const auto halfWidth = width * 0.5f;
    const auto halfHeight = height * 0.5f;

    return Matrix3x4(
        halfWidth,      0.0f,           0.0f,
        0.0f,           -halfHeight,    0.0f,
        0.0f,           0.0f,           maxZ - minZ,
        x + halfWidth,  y + halfHeight, minZ
    );
}

inline Matrix3x4 Matrix3x4::operator+(const Matrix3x4& rhs) const noexcept
{
    return Matrix3x4(
        m00 + rhs.m00, m01 + rhs.m01, m02 + rhs.m02,
        m10 + rhs.m10, m11 + rhs.m11, m12 + rhs.m12,
        m20 + rhs.m20, m21 + rhs.m21, m22 + rhs.m22,
        m30 + rhs.m30, m31 + rhs.m31, m32 + rhs.m32
    );
}

inline Matrix3x4& Matrix3x4::operator*=(const Matrix3x4& rhs) noexcept
{
    *this = (*this) * rhs;
    return *this;
}

constexpr bool Matrix3x4::operator==(const Matrix3x4& rhs) const noexcept
{
    for (int32_t i = 0; i < 12; ++i)
    {
        if ((&(m00))[i] != (&(rhs.m00))[i])
        {
            return false;
        }
    }
    return true;
}

constexpr bool Matrix3x4::operator!=(const Matrix3x4& rhs) const noexcept
{
    return !this->operator==(rhs);
}

inline float& Matrix3x4::operator[](std::size_t index)
{
    return *(&m00 + index);
}

inline float Matrix3x4::operator[](std::size_t index) const
{
    return const_cast<decltype(this)>(this)->operator[](index);
}

constexpr Matrix3x4 Matrix3x4::Translate(float x, float y, float z) noexcept
{
    return Matrix3x4(
        1.0f,   0.0f,   0.0f,
        0.0f,   1.0f,   0.0f,
        0.0f,   0.0f,   1.0f,
        x,      y,      z
    );
}

constexpr Matrix3x4 Matrix3x4::Identity() noexcept
{
    return Matrix3x4(
        1.0f,   0.0f,   0.0f,
        0.0f,   1.0f,   0.0f,
        0.0f,   0.0f,   1.0f,
        0.0f,   0.0f,   0.0f
    );
}

constexpr Matrix3x4 Matrix3x4::Zero() noexcept
{
    return Matrix3x4(
        0.0f,   0.0f,   0.0f,
        0.0f,   0.0f,   0.0f,
        0.0f,   0.0f,   0.0f,
        0.0f,   0.0f,   0.0f
    );
}

inline Matrix3x4 Matrix3x4::Rotate(float yaw, float pitch, float roll) noexcept
{
    const auto cosA = std::cos(yaw);
    const auto sinA = std::sin(yaw);
    
    const auto cosB = std::cos(pitch);
    const auto sinB = std::sin(pitch);
    
    const auto cosC = std::cos(roll);
    const auto sinC = std::sin(roll);
    
    return Matrix3x4(
        cosB * cosC,                            cosB * -sinC,                           sinB,
        (sinA * sinB * cosC) + (cosA * sinC),   (sinA * sinB * -sinC) + (cosA * cosC),  -sinA * cosB,
        (cosA * -sinB * cosC) + (sinA * sinC),  (cosA * sinB * sinC) + (sinA * cosC),   cosA * cosB,
        0.0f,   0.0f,   0.0f
    );
}

inline Matrix3x4 Matrix3x4::RotateX(float radian) noexcept
{
    const auto cosA = std::cos(radian);
    const auto sinA = std::sin(radian);

    return Matrix3x4(
        1.0f,   0.0f,   0.0f,
        0.0f,   cosA,   -sinA,
        0.0f,   sinA,   cosA,
        0.0f,   0.0f,   0.0f
    );
}

inline Matrix3x4 Matrix3x4::RotateY(float radian) noexcept
{
    const auto cosA = std::cos(radian);
    const auto sinA = std::sin(radian);

    return Matrix3x4(
        cosA,   0.0f,   sinA,
        0.0f,   1.0f,   0.0f,
        -sinA,  0.0f,   cosA,
        0.0f,   0.0f,   0.0f
    );
}

inline Matrix3x4 Matrix3x4::RotateZ(float radian) noexcept
{
    const auto cosA = std::cos(radian);
    const auto sinA = std::sin(radian);

    return Matrix3x4(
        cosA,   -sinA,   0.0f,
        sinA,   cosA,   0.0f,
        0.0f,   0.0f,   1.0f,
        0.0f,   0.0f,   0.0f
    );
}

constexpr Matrix3x4 Matrix3x4::Scale(float x, float y, float z) noexcept
{
    return Matrix3x4(
        x,      0.0f,   0.0f,
        0.0f,   y,      0.0f,
        0.0f,   0.0f,   z,
        0.0f,   0.0f,   0.0f
    );
}

inline int32_t Matrix3x4::ToString(const std::span<char>& destStr) const
{
    return this->ToString(&destStr[0], destStr.size());
}

inline int32_t Matrix3x4::ToString(char* destStr, std::size_t destStrBufferLen) const
{
    const auto destStrLen = fmt::format_to_n(destStr, sizeof(destStr[0]) * (destStrBufferLen - 1), "{}\t\t{}\t\t{}\n{}\t\t{}\t\t{}\n{}\t\t{}\t\t{}\n{}\t\t{}\t\t{}", m00, m01, m02, m10, m11, m12, m20, m21, m22, m30, m31, m32).size;
    destStr[destStrLen] = u8'\0';

    return static_cast<int32_t>(destStrLen);
}

inline std::string Matrix3x4::ToString() const
{
    std::array<char, 2048> str{};
    const auto strLen = this->ToString(str);

    return {&str[0], static_cast<size_t>(strLen)};
}

}