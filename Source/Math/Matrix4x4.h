#pragma once

#include <cstdint>
#include <cmath>
#include <span>
#include <array>

#include "Core/Simd.h"

#include "Vector3.h"

namespace tg
{

struct alignas(16) Matrix4x4
{
/**@section Constructor */
public:
    constexpr Matrix4x4() noexcept;
    constexpr Matrix4x4(float m00, float m01, float m02, float m03,
                        float m10, float m11, float m12, float m13,
                        float m20, float m21, float m22, float m23,
                        float m30, float m31, float m32, float m33) noexcept;
    explicit Matrix4x4(const std::span<const float>& m) noexcept;
    template <typename _Operator, typename _FirstOperand, typename _SecondOperand>
    constexpr Matrix4x4(const ExpressionTemplates<_Operator, _FirstOperand, _SecondOperand>& expression);

/**@section Operator */
public:
    constexpr ExpressionTemplates<Add, Matrix4x4, Matrix4x4> operator+(const Matrix4x4& rhs) const noexcept;
    constexpr ExpressionTemplates<Subtract, Matrix4x4, Matrix4x4> operator-(const Matrix4x4& rhs) const noexcept;
    Matrix4x4 operator*(const Matrix4x4& rhs) const noexcept;
    Matrix4x4& operator+=(const Matrix4x4& rhs) noexcept;
    Matrix4x4& operator-=(const Matrix4x4& rhs) noexcept;
    Matrix4x4& operator*=(const Matrix4x4& rhs) noexcept;
    bool operator==(const Matrix4x4& rhs) const noexcept;
    bool operator!=(const Matrix4x4& rhs) const noexcept;
    float& operator[](int32_t index);
    float operator[](int32_t index) const;

/**@section Method */
public:
    [[nodiscard]] static constexpr Matrix4x4 Identity() noexcept;
    [[nodiscard]] static constexpr Matrix4x4 Zero() noexcept;
    [[nodiscard]] static Matrix4x4 Translate(float x, float y, float z) noexcept;
    [[nodiscard]] static Matrix4x4 Rotate(float yaw, float pitch, float roll) noexcept;
    [[nodiscard]] static Matrix4x4 RotateX(float radian) noexcept;
    [[nodiscard]] static Matrix4x4 RotateY(float radian) noexcept;
    [[nodiscard]] static Matrix4x4 RotateZ(float radian) noexcept;
    [[nodiscard]] static Matrix4x4 Scale(float x, float y, float z) noexcept;
    [[nodiscard]] static Matrix4x4 Transposed(const Matrix4x4& matrix) noexcept;
    void Transpose() noexcept;
    [[nodiscard]] static Matrix4x4 Inverse();
    [[nodiscard]] static Matrix4x4 LookAtLH(const Vector3& eyePt, const Vector3& lookAt, const Vector3& up) noexcept;
    [[nodiscard]] static Matrix4x4 LookAtRH(const Vector3& eyePt, const Vector3& lookAt, const Vector3& up) noexcept;
    [[nodiscard]] static Matrix4x4 PerspectiveLH(float fovy, float aspect, float nearZ, float farZ) noexcept;
    [[nodiscard]] static Matrix4x4 PerspectiveRH(float fovy, float aspect, float nearZ, float farZ) noexcept;
    [[nodiscard]] static Matrix4x4 OrthographicRH(float left, float right, float top, float bottom, float nearZ, float farZ) noexcept;
    [[nodiscard]] static Matrix4x4 Viewport(float x, float y, float width, float height, float minZ, float maxZ) noexcept;
    int32_t ToString(const std::span<char8_t>& destStr) const;
    int32_t ToString(char8_t* destStr, size_t destStrBufferLen) const;
    [[nodiscard]] std::u8string ToString() const;

/**@section Variable */
public:
    float m00, m01, m02, m03,
          m10, m11, m12, m13,
          m20, m21, m22, m23,
          m30, m31, m32, m33;
};

template <typename _Operator, typename _FirstOperand, typename _SecondOperand>
constexpr Matrix4x4::Matrix4x4(const ExpressionTemplates<_Operator, _FirstOperand, _SecondOperand>& expression) :
    Matrix4x4(
        expression[0], expression[1], expression[2], expression[3],
        expression[4], expression[5], expression[6], expression[7],
        expression[8], expression[9], expression[10], expression[11],
        expression[12], expression[13], expression[14], expression[15]
    )
{
}

constexpr Matrix4x4::Matrix4x4() noexcept :
    m00(1.0f), m01(0.0f), m02(0.0f), m03(0.0f),
    m10(0.0f), m11(1.0f), m12(0.0f), m13(0.0f),
    m20(0.0f), m21(0.0f), m22(1.0f), m23(0.0f),
    m30(0.0f), m31(0.0f), m32(0.0f), m33(1.0f)
{
}

inline Matrix4x4::Matrix4x4(const std::span<const float>& m) noexcept
#if TGON_SIMD_SSE2
{
    _mm_storeu_ps(&m00, _mm_loadu_ps(&m[0]));
    _mm_storeu_ps(&m10, _mm_loadu_ps(&m[4]));
    _mm_storeu_ps(&m20, _mm_loadu_ps(&m[8]));
    _mm_storeu_ps(&m30, _mm_loadu_ps(&m[12]));
}
#else
    :
    m00(m[0]), m01(m[1]), m02(m[2]), m03(m[3]),
    m10(m[4]), m11(m[5]), m12(m[6]), m13(m[7]),
    m20(m[8]), m21(m[9]), m22(m[10]), m23(m[11]),
    m30(m[12]), m31(m[13]), m32(m[14]), m33(m[15])
{
}
#endif

constexpr Matrix4x4::Matrix4x4(float m00, float m01, float m02, float m03,
                               float m10, float m11, float m12, float m13,
                               float m20, float m21, float m22, float m23,
                               float m30, float m31, float m32, float m33) noexcept :
    m00(m00), m01(m01), m02(m02), m03(m03),
    m10(m10), m11(m11), m12(m12), m13(m13),
    m20(m20), m21(m21), m22(m22), m23(m23),
    m30(m30), m31(m31), m32(m32), m33(m33)
{
}

constexpr ExpressionTemplates<Add, Matrix4x4, Matrix4x4> Matrix4x4::operator+(const Matrix4x4& rhs) const noexcept
{
    return {*this, rhs};
}

constexpr ExpressionTemplates<Subtract, Matrix4x4, Matrix4x4> Matrix4x4::operator-(const Matrix4x4& rhs) const noexcept
{
    return {*this, rhs};
}

inline Matrix4x4 Matrix4x4::operator*(const Matrix4x4& rhs) const noexcept
{
#if TGON_SIMD_SSE2
    Matrix4x4 ret;
    auto r0 = _mm_loadu_ps(&rhs.m00);
    auto r1 = _mm_loadu_ps(&rhs.m10);
    auto r2 = _mm_loadu_ps(&rhs.m20);
    auto r3 = _mm_loadu_ps(&rhs.m30);

    auto l = _mm_loadu_ps(&m00);
    r0 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(0, 0, 0, 0)), r0);
    r1 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(1, 1, 1, 1)), r1);
    r2 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(2, 2, 2, 2)), r2);
    r3 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(3, 3, 3, 3)), r3);
    _mm_storeu_ps(&ret.m00, _mm_add_ps(_mm_add_ps(r0, r1), _mm_add_ps(r2, r3)));

    l = _mm_loadu_ps(&m10);
    r0 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(0, 0, 0, 0)), r0);
    r1 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(1, 1, 1, 1)), r1);
    r2 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(2, 2, 2, 2)), r2);
    r3 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(3, 3, 3, 3)), r3);
    _mm_storeu_ps(&ret.m10, _mm_add_ps(_mm_add_ps(r0, r1), _mm_add_ps(r2, r3)));

    l = _mm_loadu_ps(&m20);
    r0 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(0, 0, 0, 0)), r0);
    r1 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(1, 1, 1, 1)), r1);
    r2 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(2, 2, 2, 2)), r2);
    r3 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(3, 3, 3, 3)), r3);
    _mm_storeu_ps(&ret.m20, _mm_add_ps(_mm_add_ps(r0, r1), _mm_add_ps(r2, r3)));

    l = _mm_loadu_ps(&m30);
    r0 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(0, 0, 0, 0)), r0);
    r1 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(1, 1, 1, 1)), r1);
    r2 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(2, 2, 2, 2)), r2);
    r3 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(3, 3, 3, 3)), r3);
    _mm_storeu_ps(&ret.m30, _mm_add_ps(_mm_add_ps(r0, r1), _mm_add_ps(r2, r3)));

    return *this;
#else
    return Matrix4x4(
        (m00 * rhs.m00) + (m01 * rhs.m10) + (m02 * rhs.m20) + (m03 * rhs.m30),
        (m00 * rhs.m01) + (m01 * rhs.m11) + (m02 * rhs.m21) + (m03 * rhs.m31),
        (m00 * rhs.m02) + (m01 * rhs.m12) + (m02 * rhs.m22) + (m03 * rhs.m32),
        (m00 * rhs.m03) + (m01 * rhs.m13) + (m02 * rhs.m23) + (m03 * rhs.m33),

        (m10 * rhs.m00) + (m11 * rhs.m10) + (m12 * rhs.m20) + (m13 * rhs.m30),
        (m10 * rhs.m01) + (m11 * rhs.m11) + (m12 * rhs.m21) + (m13 * rhs.m31),
        (m10 * rhs.m02) + (m11 * rhs.m12) + (m12 * rhs.m22) + (m13 * rhs.m32),
        (m10 * rhs.m03) + (m11 * rhs.m13) + (m12 * rhs.m23) + (m13 * rhs.m33),

        (m20 * rhs.m00) + (m21 * rhs.m10) + (m22 * rhs.m20) + (m23 * rhs.m30),
        (m20 * rhs.m01) + (m21 * rhs.m11) + (m22 * rhs.m21) + (m23 * rhs.m31),
        (m20 * rhs.m02) + (m21 * rhs.m12) + (m22 * rhs.m22) + (m23 * rhs.m32),
        (m20 * rhs.m03) + (m21 * rhs.m13) + (m22 * rhs.m23) + (m23 * rhs.m33),

        (m30 * rhs.m00) + (m31 * rhs.m10) + (m32 * rhs.m20) + (m33 * rhs.m30),
        (m30 * rhs.m01) + (m31 * rhs.m11) + (m32 * rhs.m21) + (m33 * rhs.m31),
        (m30 * rhs.m02) + (m31 * rhs.m12) + (m32 * rhs.m22) + (m33 * rhs.m32),
        (m30 * rhs.m03) + (m31 * rhs.m13) + (m32 * rhs.m23) + (m33 * rhs.m33)
    );
#endif
}

inline Matrix4x4& Matrix4x4::operator+=(const Matrix4x4& rhs) noexcept
{
#if TGON_SIMD_SSE2
    _mm_storeu_ps(&m00, _mm_add_ps(_mm_loadu_ps(&m00), _mm_loadu_ps(&rhs.m00)));
    _mm_storeu_ps(&m10, _mm_add_ps(_mm_loadu_ps(&m10), _mm_loadu_ps(&rhs.m10)));
    _mm_storeu_ps(&m20, _mm_add_ps(_mm_loadu_ps(&m20), _mm_loadu_ps(&rhs.m20)));
    _mm_storeu_ps(&m30, _mm_add_ps(_mm_loadu_ps(&m30), _mm_loadu_ps(&rhs.m30)));
#else
    m00 += rhs.m00; m01 += rhs.m01; m02 += rhs.m02; m03 += rhs.m03;
    m10 += rhs.m10; m11 += rhs.m11; m12 += rhs.m12; m13 += rhs.m13;
    m20 += rhs.m20; m21 += rhs.m21; m22 += rhs.m22; m23 += rhs.m23;
    m30 += rhs.m30; m31 += rhs.m31; m32 += rhs.m32; m33 += rhs.m33;
#endif

    return *this;
}

inline Matrix4x4& Matrix4x4::operator-=(const Matrix4x4& rhs) noexcept
{
#if TGON_SIMD_SSE2
    _mm_storeu_ps(&m00, _mm_sub_ps(_mm_loadu_ps(&m00), _mm_loadu_ps(&rhs.m00)));
    _mm_storeu_ps(&m10, _mm_sub_ps(_mm_loadu_ps(&m10), _mm_loadu_ps(&rhs.m10)));
    _mm_storeu_ps(&m20, _mm_sub_ps(_mm_loadu_ps(&m20), _mm_loadu_ps(&rhs.m20)));
    _mm_storeu_ps(&m30, _mm_sub_ps(_mm_loadu_ps(&m30), _mm_loadu_ps(&rhs.m30)));
#else
    m00 -= rhs.m00; m01 -= rhs.m01; m02 -= rhs.m02; m03 -= rhs.m03;
    m10 -= rhs.m10; m11 -= rhs.m11; m12 -= rhs.m12; m13 -= rhs.m13;
    m20 -= rhs.m20; m21 -= rhs.m21; m22 -= rhs.m22; m23 -= rhs.m23;
    m30 -= rhs.m30; m31 -= rhs.m31; m32 -= rhs.m32; m33 -= rhs.m33;
#endif

    return *this;
}

inline Matrix4x4 Matrix4x4::LookAtLH(const Vector3& eyePt, const Vector3& lookAt, const Vector3& up) noexcept
{
    Vector3 l = lookAt - eyePt;
    l.Normalize();
    
    Vector3 r = Vector3::Cross(up, l);
    r.Normalize();

    Vector3 u = Vector3::Cross(l, r);
    u.Normalize();

    return Matrix4x4(
        r.x,    u.x,    l.x,    0.0f,
        r.y,    u.y,    l.y,    0.0f,
        r.z,    u.z,    l.z,    0.0f,
        -Vector3::Dot(r, eyePt), -Vector3::Dot(u, eyePt), -Vector3::Dot(l, eyePt), 1.0f
    );
}

inline Matrix4x4 Matrix4x4::LookAtRH(const Vector3& eyePt, const Vector3& lookAt, const Vector3& up) noexcept
{
    Vector3 l = lookAt - eyePt;
    l.Normalize();

    Vector3 r = Vector3::Cross(l, up);
    r.Normalize();

    Vector3 u = Vector3::Cross(r, l);
    u.Normalize();
    
    return Matrix4x4(
        r.x,    u.x,    -l.x,   0.0f,
        r.y,    u.y,    -l.y,   0.0f,
        r.z,    u.z,    -l.z,   0.0f,
        -Vector3::Dot(r, eyePt), -Vector3::Dot(u, eyePt), Vector3::Dot(l, eyePt), 1.0f
    );
}

inline Matrix4x4 Matrix4x4::PerspectiveLH(float fovy, float aspect, float nearZ, float farZ) noexcept
{
    const float scaleY = 1.0f / std::tan(fovy * 0.5f);
    const float scaleX = scaleY / aspect;

    return Matrix4x4(
        scaleX, 0.0f,   0.0f,                               0.0f,
        0.0f,   scaleY, 0.0f,                               0.0f,
        0.0f,   0.0f,   farZ / (farZ - nearZ),              1.0f,
        0.0f,   0.0f,   -(farZ * nearZ) / (farZ - nearZ),   0.0f
    );
}

inline Matrix4x4 Matrix4x4::PerspectiveRH(float fovy, float aspect, float nearZ, float farZ) noexcept
{
    const float scaleY = 1.0f / std::tan(fovy * 0.5f);
    const float scaleX = scaleY / aspect;

    return Matrix4x4(
        scaleX, 0.0f,   0.0f,                               0.0f,
        0.0f,   scaleY, 0.0f,                               0.0f,
        0.0f,   0.0f,   farZ / (nearZ - farZ),              -1.0f,
        0.0f,   0.0f,   -(farZ * nearZ) / (farZ - nearZ),   0.0f
    );
}
    
inline Matrix4x4 Matrix4x4::OrthographicRH(float left, float right, float top, float bottom, float nearZ, float farZ) noexcept
{
    const float width = right - left;
    const float height = bottom - top;
    const float depth = farZ - nearZ;
    
    return Matrix4x4(
        2 / width,  0.0f,           0.0f,               0.0f,
        0.0f,       2 / height,     0.0f,               0.0f,
        0.0f,       0.0f,           -2.0f / depth,      0.0f,
        -(right + left) / width,    -(top + bottom) / height,   -(farZ + nearZ) / depth,    1.0f
    );
}

inline Matrix4x4 Matrix4x4::Viewport(float x, float y, float width, float height, float minZ, float maxZ) noexcept
{
    const float halfWidth = width * 0.5f;
    const float halfHeight = height * 0.5f;

    return Matrix4x4(
        halfWidth,      0.0f,           0.0f,           0.0f,
        0.0f,           -halfHeight,    0.0f,           0.0f,
        0.0f,           0.0f,           maxZ - minZ,    0.0f,
        x + halfWidth,  y + halfHeight, minZ,           1.0f
    );
}

inline Matrix4x4 Matrix4x4::Transposed(const Matrix4x4& matrix) noexcept
{
#if TGON_SIMD_SSE2
    Matrix4x4 ret;
    const auto r1 = _mm_loadu_ps(&matrix.m00);
    const auto r2 = _mm_loadu_ps(&matrix.m10);
    const auto r3 = _mm_loadu_ps(&matrix.m20);
    const auto r4 = _mm_loadu_ps(&matrix.m30);
    
    const auto r5 = _mm_shuffle_ps(r1, r2, _MM_SHUFFLE(1, 0, 1, 0));
    const auto r6 = _mm_shuffle_ps(r1, r2, _MM_SHUFFLE(3, 2, 3, 2));
    const auto r7 = _mm_shuffle_ps(r3, r4, _MM_SHUFFLE(1, 0, 1, 0));
    const auto r8 = _mm_shuffle_ps(r3, r4, _MM_SHUFFLE(3, 2, 3, 2));

    _mm_storeu_ps(&ret.m00, _mm_shuffle_ps(r5, r7, _MM_SHUFFLE(2, 0, 2, 0)));
    _mm_storeu_ps(&ret.m10, _mm_shuffle_ps(r5, r7, _MM_SHUFFLE(3, 1, 3, 1)));
    _mm_storeu_ps(&ret.m20, _mm_shuffle_ps(r6, r8, _MM_SHUFFLE(2, 0, 2, 0)));
    _mm_storeu_ps(&ret.m30, _mm_shuffle_ps(r6, r8, _MM_SHUFFLE(3, 1, 3, 1)));

    return ret;
#else
    return Matrix4x4(
        matrix.m00, matrix.m10, matrix.m20, matrix.m30,
        matrix.m01, matrix.m11, matrix.m21, matrix.m31,
        matrix.m02, matrix.m12, matrix.m22, matrix.m32,
        matrix.m03, matrix.m13, matrix.m23, matrix.m33
    );
#endif
}

inline void Matrix4x4::Transpose() noexcept
{
#if TGON_SIMD_SSE2
    const auto r1 = _mm_loadu_ps(&m00);
    const auto r2 = _mm_loadu_ps(&m10);
    const auto r3 = _mm_loadu_ps(&m20);
    const auto r4 = _mm_loadu_ps(&m30);
    
    const auto r5 = _mm_shuffle_ps(r1, r2, _MM_SHUFFLE(1, 0, 1, 0));
    const auto r6 = _mm_shuffle_ps(r1, r2, _MM_SHUFFLE(3, 2, 3, 2));
    const auto r7 = _mm_shuffle_ps(r3, r4, _MM_SHUFFLE(1, 0, 1, 0));
    const auto r8 = _mm_shuffle_ps(r3, r4, _MM_SHUFFLE(3, 2, 3, 2));

    _mm_storeu_ps(&m00, _mm_shuffle_ps(r5, r7, _MM_SHUFFLE(2, 0, 2, 0)));
    _mm_storeu_ps(&m10, _mm_shuffle_ps(r5, r7, _MM_SHUFFLE(3, 1, 3, 1)));
    _mm_storeu_ps(&m20, _mm_shuffle_ps(r6, r8, _MM_SHUFFLE(2, 0, 2, 0)));
    _mm_storeu_ps(&m30, _mm_shuffle_ps(r6, r8, _MM_SHUFFLE(3, 1, 3, 1)));
#else
    *this = Transposed(*this);
#endif

}

inline Matrix4x4& Matrix4x4::operator*=(const Matrix4x4& rhs) noexcept
{
#if TGON_SIMD_SSE2
    auto r0 = _mm_loadu_ps(&rhs.m00);
    auto r1 = _mm_loadu_ps(&rhs.m10);
    auto r2 = _mm_loadu_ps(&rhs.m20);
    auto r3 = _mm_loadu_ps(&rhs.m30);

    auto l = _mm_loadu_ps(&m00);
    r0 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(0, 0, 0, 0)), r0);
    r1 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(1, 1, 1, 1)), r1);
    r2 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(2, 2, 2, 2)), r2);
    r3 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(3, 3, 3, 3)), r3);
    _mm_storeu_ps(&m00, _mm_add_ps(_mm_add_ps(r0, r1), _mm_add_ps(r2, r3)));

    l = _mm_loadu_ps(&m10);
    r0 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(0, 0, 0, 0)), r0);
    r1 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(1, 1, 1, 1)), r1);
    r2 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(2, 2, 2, 2)), r2);
    r3 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(3, 3, 3, 3)), r3);
    _mm_storeu_ps(&m10, _mm_add_ps(_mm_add_ps(r0, r1), _mm_add_ps(r2, r3)));

    l = _mm_loadu_ps(&m20);
    r0 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(0, 0, 0, 0)), r0);
    r1 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(1, 1, 1, 1)), r1);
    r2 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(2, 2, 2, 2)), r2);
    r3 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(3, 3, 3, 3)), r3);
    _mm_storeu_ps(&m20, _mm_add_ps(_mm_add_ps(r0, r1), _mm_add_ps(r2, r3)));

    l = _mm_loadu_ps(&m30);
    r0 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(0, 0, 0, 0)), r0);
    r1 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(1, 1, 1, 1)), r1);
    r2 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(2, 2, 2, 2)), r2);
    r3 = _mm_mul_ps(_mm_shuffle_ps(l, l, _MM_SHUFFLE(3, 3, 3, 3)), r3);
    _mm_storeu_ps(&m30, _mm_add_ps(_mm_add_ps(r0, r1), _mm_add_ps(r2, r3)));
#else
    *this = Matrix4x4(
        (m00 * rhs.m00) + (m01 * rhs.m10) + (m02 * rhs.m20) + (m03 * rhs.m30),
        (m00 * rhs.m01) + (m01 * rhs.m11) + (m02 * rhs.m21) + (m03 * rhs.m31),
        (m00 * rhs.m02) + (m01 * rhs.m12) + (m02 * rhs.m22) + (m03 * rhs.m32),
        (m00 * rhs.m03) + (m01 * rhs.m13) + (m02 * rhs.m23) + (m03 * rhs.m33),

        (m10 * rhs.m00) + (m11 * rhs.m10) + (m12 * rhs.m20) + (m13 * rhs.m30),
        (m10 * rhs.m01) + (m11 * rhs.m11) + (m12 * rhs.m21) + (m13 * rhs.m31),
        (m10 * rhs.m02) + (m11 * rhs.m12) + (m12 * rhs.m22) + (m13 * rhs.m32),
        (m10 * rhs.m03) + (m11 * rhs.m13) + (m12 * rhs.m23) + (m13 * rhs.m33),

        (m20 * rhs.m00) + (m21 * rhs.m10) + (m22 * rhs.m20) + (m23 * rhs.m30),
        (m20 * rhs.m01) + (m21 * rhs.m11) + (m22 * rhs.m21) + (m23 * rhs.m31),
        (m20 * rhs.m02) + (m21 * rhs.m12) + (m22 * rhs.m22) + (m23 * rhs.m32),
        (m20 * rhs.m03) + (m21 * rhs.m13) + (m22 * rhs.m23) + (m23 * rhs.m33),

        (m30 * rhs.m00) + (m31 * rhs.m10) + (m32 * rhs.m20) + (m33 * rhs.m30),
        (m30 * rhs.m01) + (m31 * rhs.m11) + (m32 * rhs.m21) + (m33 * rhs.m31),
        (m30 * rhs.m02) + (m31 * rhs.m12) + (m32 * rhs.m22) + (m33 * rhs.m32),
        (m30 * rhs.m03) + (m31 * rhs.m13) + (m32 * rhs.m23) + (m33 * rhs.m33)
    );
#endif
    return *this;
}

inline bool Matrix4x4::operator==(const Matrix4x4& rhs) const noexcept
{
    for (auto i = 0; i < 16; ++i)
    {
        if ((&(m00))[i] != (&(rhs.m00))[i])
        {
            return false;
        }
    }
    return true;
}

inline bool Matrix4x4::operator!=(const Matrix4x4& rhs) const noexcept
{
    return !this->operator==(rhs);
}

inline float& Matrix4x4::operator[](int32_t index)
{
    return *(&m00 + index);
}

inline float Matrix4x4::operator[](int32_t index) const
{
    return const_cast<decltype(this)>(this)->operator[](index);
}

inline Matrix4x4 Matrix4x4::Translate(float x, float y, float z) noexcept
{
    return Matrix4x4(
        1.0f,   0.0f,   0.0f,   0.0f,
        0.0f,   1.0f,   0.0f,   0.0f,
        0.0f,   0.0f,   1.0f,   0.0f,
        x,      y,      z,      1.0f
    );
}

constexpr Matrix4x4 Matrix4x4::Identity() noexcept
{
    return Matrix4x4(
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    );
}

constexpr Matrix4x4 Matrix4x4::Zero() noexcept
{
    return Matrix4x4(
        0.0f,   0.0f,   0.0f,   0.0f,
        0.0f,   0.0f,   0.0f,   0.0f,
        0.0f,   0.0f,   0.0f,   0.0f,
        0.0f,   0.0f,   0.0f,   0.0f
    );
}

inline Matrix4x4 Matrix4x4::Rotate(float yaw, float pitch, float roll) noexcept
{
    const float cosA = std::cos(yaw);
    const float sinA = std::sin(yaw);
    
    const float cosB = std::cos(pitch);
    const float sinB = std::sin(pitch);
    
    const float cosC = std::cos(roll);
    const float sinC = std::sin(roll);
    
    return Matrix4x4(
        cosB * cosC,                            cosB * -sinC,                           sinB,           0.0f,
        (sinA * sinB * cosC) + (cosA * sinC),   (sinA * sinB * -sinC) + (cosA * cosC),  -sinA * cosB,   0.0f,
        (cosA * -sinB * cosC) + (sinA * sinC),  (cosA * sinB * sinC) + (sinA * cosC),   cosA * cosB,    0.0f,
        0.0f,   0.0f,   0.0f,   1.0f
    );
}

inline Matrix4x4 Matrix4x4::RotateX(float radian) noexcept
{
    const float cosA = std::cos(radian);
    const float sinA = std::sin(radian);

    return Matrix4x4(
        1.0f,   0.0f,   0.0f,   0.0f,
        0.0f,   cosA,   -sinA,  0.0f,
        0.0f,   sinA,   cosA,   0.0f,
        0.0f,   0.0f,   0.0f,   1.0f
    );
}

inline Matrix4x4 Matrix4x4::RotateY(float radian) noexcept
{
    const float cosA = std::cos(radian);
    const float sinA = std::sin(radian);

    return Matrix4x4(
        cosA,   0.0f,   sinA,   0.0f,
        0.0f,   1.0f,   0.0f,   0.0f,
        -sinA,  0.0f,   cosA,   0.0f,
        0.0f,   0.0f,   0.0f,   1.0f
    );
}

inline Matrix4x4 Matrix4x4::RotateZ(float radian) noexcept
{
    const float cosA = std::cos(radian);
    const float sinA = std::sin(radian);

    return Matrix4x4(
        cosA,   -sinA,  0.0f,   0.0f,
        sinA,   cosA,   0.0f,   0.0f,
        0.0f,   0.0f,   1.0f,   0.0f,
        0.0f,   0.0f,   0.0f,   1.0f
    );
}

inline Matrix4x4 Matrix4x4::Scale(float x, float y, float z) noexcept
{
    return Matrix4x4(
        x,      0.0f,   0.0f,   0.0f,
        0.0f,   y,      0.0f,   0.0f,
        0.0f,   0.0f,   z,      0.0f,
        0.0f,   0.0f,   0.0f,   1.0f
    );
}

inline int32_t Matrix4x4::ToString(const std::span<char8_t>& destStr) const
{
    return this->ToString(&destStr[0], destStr.size());
}

inline int32_t Matrix4x4::ToString(char8_t* destStr, size_t destStrBufferLen) const
{
    const auto destStrLen = fmt::format_to_n(destStr, sizeof(destStr[0]) * (destStrBufferLen - 1), "{}\t\t{}\t\t{}\t\t{}\n{}\t\t{}\t\t{}\t\t{}\n{}\t\t{}\t\t{}\t\t{}\n{}\t\t{}\t\t{}\t\t{}", m00, m01, m02, m03, m10, m11, m12, m13, m20, m21, m22, m23, m30, m31, m32, m33).size;
    destStr[destStrLen] = u8'\0';

    return static_cast<int32_t>(destStrLen);
}

inline std::u8string Matrix4x4::ToString() const
{
    std::array<char8_t, 2048> str{};
    const int32_t strLen = this->ToString(str);

    return {&str[0], static_cast<size_t>(strLen)};
}

}