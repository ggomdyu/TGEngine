#pragma once

#include <cstdint>
#include <type_traits>

TG_NAMESPACE_BEGIN

class IComparable
{
public:
    IComparable(const IComparable&) = default;
    IComparable(IComparable&&) = default;
    virtual ~IComparable() = default;

public:
    IComparable& operator=(const IComparable&) = default;
    IComparable& operator=(IComparable&&) = default;
    bool operator==(const IComparable* rhs) const;
    bool operator!=(const IComparable* rhs) const;

public:
    virtual bool Equals(const IComparable* rhs) const;
    virtual int32_t CompareTo(const IComparable* rhs) const = 0;
};

inline bool IComparable::operator==(const IComparable* rhs) const
{
    return this->Equals(rhs);
}
    
inline bool IComparable::operator!=(const IComparable* rhs) const
{
    return !this->Equals(rhs);
}

inline bool IComparable::Equals(const IComparable* rhs) const
{
    return this->CompareTo(rhs) == 0;
}

TG_NAMESPACE_END