#pragma once

#include "Rtti.h"

TG_NAMESPACE_BEGIN

class RuntimeObject
{
public:
    using Super = void;
    using This = RuntimeObject;

public:
    RuntimeObject() noexcept = default;
    RuntimeObject(const RuntimeObject& rhs) = delete;
    RuntimeObject(RuntimeObject&& rhs) = default;
    virtual ~RuntimeObject() = default;

public:
    RuntimeObject& operator=(const RuntimeObject& rhs) = delete;
    RuntimeObject& operator=(RuntimeObject&& rhs) = default;

public:
    virtual const Rtti* GetRtti() const = 0;
};

TG_NAMESPACE_END

#define TG_OBJECT(classType)\
    using Super = This;\
    using This = classType;\
    \
    const tg::Rtti* GetRtti() const override\
    {\
        static_assert(std::is_same_v<std::remove_const_t<std::remove_pointer_t<decltype(this)>>, This>, "TG_OBJECT didn't receive the owner type as a parameter.");\
        return tg::GetRtti<classType>();\
    }
