#pragma once

#include <algorithm>

#include "Platform/Windows/Windows.h"

namespace tg
{

class SafeFindHandle final
{
/**@section Constructor */
public:
    constexpr SafeFindHandle() noexcept = default;
    constexpr explicit SafeFindHandle(HANDLE handle) noexcept;
    SafeFindHandle(const SafeFindHandle& rhs) = delete;
    SafeFindHandle(SafeFindHandle&& rhs) noexcept;

/**@section Destructor */
public:
    ~SafeFindHandle();

/**@section Operator */
public:
    SafeFindHandle& operator=(const SafeFindHandle& rhs) = delete;
    SafeFindHandle& operator=(SafeFindHandle&& rhs) noexcept;
    constexpr bool operator==(const SafeFindHandle& rhs) const noexcept;
    constexpr bool operator!=(const SafeFindHandle& rhs) const noexcept;
    constexpr bool operator==(std::nullptr_t rhs) const noexcept;
    constexpr bool operator!=(std::nullptr_t rhs) const noexcept;
    operator HANDLE() const noexcept;
    
/**@section Variable */
private:
    HANDLE m_handle = INVALID_HANDLE_VALUE;
};

constexpr SafeFindHandle::SafeFindHandle(HANDLE handle) noexcept :
    m_handle(handle)
{
}

inline SafeFindHandle::SafeFindHandle(SafeFindHandle&& rhs) noexcept :
    m_handle(rhs.m_handle)
{
    rhs.m_handle = INVALID_HANDLE_VALUE;
}

inline SafeFindHandle::~SafeFindHandle()
{
    if (m_handle != nullptr)
    {
        FindClose(m_handle);
    }
}

inline SafeFindHandle& SafeFindHandle::operator=(SafeFindHandle&& rhs) noexcept
{
    std::swap(m_handle, rhs.m_handle);
    return *this;
}

constexpr bool SafeFindHandle::operator==(const SafeFindHandle& rhs) const noexcept
{
    return m_handle == rhs.m_handle;
}

constexpr bool SafeFindHandle::operator!=(const SafeFindHandle& rhs) const noexcept
{
    return m_handle != rhs.m_handle;
}

constexpr bool SafeFindHandle::operator==(std::nullptr_t) const noexcept
{
    return m_handle == INVALID_HANDLE_VALUE;
}

constexpr bool SafeFindHandle::operator!=(std::nullptr_t) const noexcept
{
    return m_handle != INVALID_HANDLE_VALUE;
}

inline SafeFindHandle::operator HANDLE() const noexcept
{
    return m_handle;
}

}