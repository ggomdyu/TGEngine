#pragma once

#include <algorithm>

#include "Platform/Windows/Windows.h"

namespace tg
{

class SafeHandle final
{
/**@section Constructor */
public:
    constexpr SafeHandle() noexcept = default;
    constexpr explicit SafeHandle(HANDLE handle) noexcept;
    SafeHandle(const SafeHandle& rhs) = delete;
    SafeHandle(SafeHandle&& rhs) noexcept;

/**@section Destructor */
public:
    ~SafeHandle();

/**@section Operator */
public:
    SafeHandle& operator=(const SafeHandle& rhs) = delete;
    SafeHandle& operator=(SafeHandle&& rhs) noexcept;
    constexpr bool operator==(const SafeHandle& rhs) const noexcept;
    constexpr bool operator!=(const SafeHandle& rhs) const noexcept;
    constexpr bool operator==(std::nullptr_t rhs) const noexcept;
    constexpr bool operator!=(std::nullptr_t rhs) const noexcept;
    operator HANDLE() const noexcept;
    
/**@section Variable */
private:
    HANDLE m_handle = INVALID_HANDLE_VALUE;
};

constexpr SafeHandle::SafeHandle(HANDLE handle) noexcept :
    m_handle(handle)
{
}

inline SafeHandle::SafeHandle(SafeHandle&& rhs) noexcept :
    m_handle(rhs.m_handle)
{
    rhs.m_handle = INVALID_HANDLE_VALUE;
}

inline SafeHandle::~SafeHandle()
{
    if (m_handle != nullptr)
    {
        CloseHandle(m_handle);
    }
}

inline SafeHandle& SafeHandle::operator=(SafeHandle&& rhs) noexcept
{
    std::swap(m_handle, rhs.m_handle);
    return *this;
}

constexpr bool SafeHandle::operator==(const SafeHandle& rhs) const noexcept
{
    return m_handle == rhs.m_handle;
}

constexpr bool SafeHandle::operator!=(const SafeHandle& rhs) const noexcept
{
    return m_handle != rhs.m_handle;
}

constexpr bool SafeHandle::operator==(std::nullptr_t) const noexcept
{
    return m_handle == INVALID_HANDLE_VALUE;
}

constexpr bool SafeHandle::operator!=(std::nullptr_t) const noexcept
{
    return m_handle != INVALID_HANDLE_VALUE;
}

inline SafeHandle::operator HANDLE() const noexcept
{
    return m_handle;
}

}