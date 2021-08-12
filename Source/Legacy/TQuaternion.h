/**
 * @filename    Quaternion.h
 * @author      ggomdyu
 * @since       03/14/2016
 */

#pragma once
#include "Core/Platform/Config.h"

#include "Vector3.h"

namespace tgon {
namespace math {

struct TGON_API Quaternion
{
/**
 * @section Ctor/Dtor
 */
public:
    Quaternion();

    // In Quaternion, x, y, z means a Axis. w is angle scalar.
    Quaternion(float x, float y, float z, float w);

    // Pass axis as Vector3. w is angle scalar.
    Quaternion(const Vector3& v, float w);

    Quaternion(const Quaternion&);
    
/**
 * @section Operator
 */
public:
    Quaternion operator*(const Quaternion&) const;
    Quaternion operator+() const;
    Quaternion operator-() const;
    Quaternion& operator*=(const Quaternion&);
    bool operator==(const Quaternion&) const;
    bool operator!=(const Quaternion&) const;
    float& operator[](std::size_t);
    float operator[](std::size_t) const;
	
/**
 * @section Public command method
 */
public:
    struct TMatrix4x4 RotateMatrix() const;

/**
 * @section Public variables
 */
public:
	float x, y, z, w;

    static const Quaternion Zero;
    static const Quaternion Identity;
};

inline Quaternion::Quaternion() :
    x(0.f),
    y(0.f),
    z(0.f),
    w(0.f)
{
}

inline Quaternion::Quaternion(float x, float y, float z, float w) :
    x(x),
    y(y),
    z(z),
    w(w)
{
}

inline Quaternion::Quaternion(const Quaternion& rhs) :
    x(rhs.x),
    y(rhs.y),
    z(rhs.z),
    w(rhs.w)
{
}

inline Quaternion Quaternion::operator*(const Quaternion& rhs) const
{
    return Quaternion(x*rhs.x, y*rhs.y, z*rhs.z, w*rhs.w);
}

inline Quaternion Quaternion::operator+() const
{
    return *this;
}

inline Quaternion Quaternion::operator-() const
{
    return Quaternion(-x, -y, -z, -w);
}

inline Quaternion& Quaternion::operator*=(const Quaternion& rhs)
{
    x *= rhs.x;
    y *= rhs.y;
    z *= rhs.z;
    w *= rhs.w;

    return *this;
}

inline bool Quaternion::operator==(const Quaternion& rhs) const
{
    return (x == rhs.x && y == rhs.y && z == rhs.z && w == rhs.w);
}

inline bool Quaternion::operator!=(const Quaternion& rhs) const
{
    return !(this->operator==(rhs));
}

inline float& Quaternion::operator[](std::size_t index)
{
    assert((index < 4 && index > -1) && "Quaternion index out of range");
    return *(&x + index);
}

inline float Quaternion::operator[](std::size_t index) const
{
    assert((index < 4 && index > -1) && "Quaternion index out of range");
    return *(&x + index);
}

} /* namespace math */
} /* namespace tgon */
