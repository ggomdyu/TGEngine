/**
 * @file    HandleGuard.h
 * @author  ggomdyu
 * @since   05/17/2016
 */

#pragma once
#include <Windows.h>
#include <boost/noncopyable.hpp>

#include "../RAII.h"

namespace tgon
{

/**
 * @class   HandleGuard
 * @brief   RAII object that manages the reference count of windows handle automatically.
 */
class HandleGuard final :
    public RAII<HANDLE, HandleGuard>,
    private boost::noncopyable
{
/**@section Constructor */
public:
    /**@brief   Adds the reference count of managed resource. */
    void AddRef();

    /**@brief   Releases the managed resource. */
    void Release(HANDLE& resource);

    /**@brief   Returns special value which indicates resource is null. */
    HANDLE GetNullValue() const noexcept;
};

inline void HandleGuard::AddRef()
{
}

inline void HandleGuard::Release(HANDLE& resource)
{
    if (resource != GetNullValue())
    {
        CloseHandle(resource);
        resource = GetNullValue();
    }
}

inline HANDLE HandleGuard::GetNullValue() const noexcept
{
    return INVALID_HANDLE_VALUE;
}

} /* namespace tgon */