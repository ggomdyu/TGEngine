#pragma once

#include <algorithm>

#include "Platform/Windows/Windows.h"

namespace tg
{

class SafeRegistryHandle final
{
/**@section Constructor */
public:
    constexpr SafeRegistryHandle() noexcept = default;
    constexpr explicit SafeRegistryHandle(HKEY keyHandle) noexcept;
    SafeRegistryHandle(const SafeRegistryHandle& rhs) = delete;
    SafeRegistryHandle(SafeRegistryHandle&& rhs) noexcept;

/**@section Destructor */
public:
    ~SafeRegistryHandle();

/**@section Operator */
public:
    SafeRegistryHandle& operator=(const SafeRegistryHandle& rhs) = delete;
    SafeRegistryHandle& operator=(SafeRegistryHandle&& rhs) noexcept;
    constexpr bool operator==(const SafeRegistryHandle& rhs) const noexcept;
    constexpr bool operator!=(const SafeRegistryHandle& rhs) const noexcept;
    constexpr bool operator==(std::nullptr_t rhs) const noexcept;
    constexpr bool operator!=(std::nullptr_t rhs) const noexcept;
    operator PHKEY() noexcept;
    operator HKEY() const noexcept;
    
/**@section Variable */
private:
    HKEY m_keyHandle = nullptr;
};

constexpr SafeRegistryHandle::SafeRegistryHandle(HKEY keyHandle) noexcept :
    m_keyHandle(keyHandle)
{
}

inline SafeRegistryHandle::SafeRegistryHandle(SafeRegistryHandle&& rhs) noexcept :
    m_keyHandle(rhs.m_keyHandle)
{
    rhs.m_keyHandle = nullptr;
}

inline SafeRegistryHandle::~SafeRegistryHandle()
{
    if (m_keyHandle != nullptr)
    {
        CloseHandle(m_keyHandle);
    }
}

inline SafeRegistryHandle& SafeRegistryHandle::operator=(SafeRegistryHandle&& rhs) noexcept
{
    std::swap(m_keyHandle, rhs.m_keyHandle);
    return *this;
}

constexpr bool SafeRegistryHandle::operator==(const SafeRegistryHandle& rhs) const noexcept
{
    return m_keyHandle == rhs.m_keyHandle;
}

constexpr bool SafeRegistryHandle::operator!=(const SafeRegistryHandle& rhs) const noexcept
{
    return m_keyHandle != rhs.m_keyHandle;
}

constexpr bool SafeRegistryHandle::operator==(std::nullptr_t) const noexcept
{
    return m_keyHandle == nullptr;
}

constexpr bool SafeRegistryHandle::operator!=(std::nullptr_t) const noexcept
{
    return m_keyHandle != nullptr;
}

inline SafeRegistryHandle::operator PHKEY() noexcept
{
    return &m_keyHandle;
}

inline SafeRegistryHandle::operator HKEY() const noexcept
{
    return m_keyHandle;
}

}