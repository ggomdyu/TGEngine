#pragma once

#include <typeinfo>

#include "TypeTraits.h"

TG_NAMESPACE_BEGIN

class Rtti final
{
public:
    Rtti(const std::type_info& typeInfo, const Rtti* superRtti) noexcept;

public:
    bool operator==(const Rtti& rhs) const noexcept;
    bool operator!=(const Rtti& rhs) const noexcept;
    bool operator<(const Rtti& rhs) const noexcept;
    bool operator>=(const Rtti& rhs) const noexcept;
    bool operator>(const Rtti& rhs) const noexcept;
    bool operator<=(const Rtti& rhs) const noexcept;

public:
    size_t GetHashCode() const noexcept;
    const char* GetName() const noexcept;
    const Rtti* GetSuperRtti() const noexcept;

private:
    const std::type_info* _typeInfo;
    const Rtti* _superRtti;
};

template <typename _Type>
std::enable_if_t<IsRawType<_Type>, const Rtti*> GetRtti()
{
    using RawType = RawType<_Type>;

    static const Rtti rtti(typeid(RawType), GetRtti<typename RawType::Super>());
    return &rtti;
}

template <typename _Type>
std::enable_if_t<!IsRawType<_Type>, const Rtti*> GetRtti()
{
    return GetRtti<RawType<_Type>>();
}

template <>
inline const Rtti* GetRtti<void>()
{
    return nullptr;
}

inline Rtti::Rtti(const std::type_info& typeInfo, const Rtti* superRtti) noexcept :
    _typeInfo(&typeInfo),
    _superRtti(superRtti)
{
}

inline size_t Rtti::GetHashCode() const noexcept
{
    return _typeInfo->hash_code();
}

inline const char* Rtti::GetName() const noexcept
{
    return _typeInfo->name();
}

inline const Rtti* Rtti::GetSuperRtti() const noexcept
{
    return _superRtti;
}

inline bool Rtti::operator==(const Rtti& rhs) const noexcept
{
    return _typeInfo == rhs._typeInfo;
}

inline bool Rtti::operator!=(const Rtti& rhs) const noexcept
{
    return !(*this == rhs);
}

inline bool Rtti::operator<(const Rtti& rhs) const noexcept
{
    return _typeInfo->before(*rhs._typeInfo);
}

inline bool Rtti::operator>=(const Rtti& rhs) const noexcept
{
    return !(*this < rhs);
}

inline bool Rtti::operator>(const Rtti& rhs) const noexcept
{
    return (rhs < *this);
}

inline bool Rtti::operator<=(const Rtti& rhs) const noexcept
{
    return !(*this > rhs);
}

}

namespace std
{

template <>
struct hash<tg::Rtti>
{
    size_t operator()(const tg::Rtti& rhs) const noexcept;
};

inline size_t hash<tg::Rtti>::operator()(const tg::Rtti& rhs) const noexcept
{
    return rhs.GetHashCode();
}
    
}
