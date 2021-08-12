#pragma once

#include "Rtti.h"

namespace tg
{

class RuntimeObject
{
/**@section Type */
public:
    using Super = void;
    using This = RuntimeObject;

/**@section Constructor */
public:
    RuntimeObject() noexcept = default;
    RuntimeObject(const RuntimeObject& rhs) = delete;
    RuntimeObject(RuntimeObject&& rhs) = default;

/**@section Destructor */
public:
    virtual ~RuntimeObject() = default;

/**@section Operator */
public:
    RuntimeObject& operator=(const RuntimeObject& rhs) = delete;
    RuntimeObject& operator=(RuntimeObject&& rhs) = default;

/**@section Method */
public:
    [[nodiscard]] virtual const Rtti* GetRtti() const = 0;
};

}

#define TGON_RTTI(classType)\
    using Super = This;\
    using This = classType;\
    \
    const tg::Rtti* GetRtti() const override\
    {\
        static_assert(std::is_same_v<std::remove_const_t<std::remove_pointer_t<decltype(this)>>, This>, "TGON_RTTI didn't receive the owner type as a parameter.");\
        return tg::GetRtti<classType>();\
    }
