#pragma once

#include <typeinfo>

#include "TypeTraits.h"

namespace tg
{

class Rtti final
{
/**@section Constructor */
public:
    Rtti(const std::type_info& typeInfo, const Rtti* superRtti) noexcept;

/**@section Operator */
public:
    bool operator==(const Rtti& rhs) const noexcept;
    bool operator!=(const Rtti& rhs) const noexcept;
    bool operator<(const Rtti& rhs) const noexcept;
    bool operator>=(const Rtti& rhs) const noexcept;
    bool operator>(const Rtti& rhs) const noexcept;
    bool operator<=(const Rtti& rhs) const noexcept;

/**@section Method */
public:
    [[nodiscard]] size_t GetHashCode() const noexcept;
    [[nodiscard]] const char* GetName() const noexcept;
    [[nodiscard]] const Rtti* GetSuperRtti() const noexcept;

/**@section Variable */
private:
    const std::type_info* m_typeInfo;
    const Rtti* m_superRtti;
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
    m_typeInfo(&typeInfo),
    m_superRtti(superRtti)
{
}

inline size_t Rtti::GetHashCode() const noexcept
{
    return m_typeInfo->hash_code();
}

inline const char* Rtti::GetName() const noexcept
{
    return m_typeInfo->name();
}

inline const Rtti* Rtti::GetSuperRtti() const noexcept
{
    return m_superRtti;
}

inline bool Rtti::operator==(const Rtti& rhs) const noexcept
{
    return m_typeInfo == rhs.m_typeInfo;
}

inline bool Rtti::operator!=(const Rtti& rhs) const noexcept
{
    return !(*this == rhs);
}

inline bool Rtti::operator<(const Rtti& rhs) const noexcept
{
    return m_typeInfo->before(*rhs.m_typeInfo);
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
