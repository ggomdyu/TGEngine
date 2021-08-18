#pragma once

#include <memory>

TG_NAMESPACE_BEGIN

class ICloneable
{
public:
    ICloneable() = default;
    ICloneable(const ICloneable&) = delete;
    ICloneable(ICloneable&&) = delete;
    virtual ~ICloneable() = default;

public:
    ICloneable& operator=(const ICloneable& rhs) noexcept = delete;
    ICloneable& operator=(ICloneable&& rhs) noexcept = delete;

public:
    virtual std::shared_ptr<ICloneable> Clone() = 0;
    std::shared_ptr<const ICloneable> Clone() const;
};

inline std::shared_ptr<const ICloneable> ICloneable::Clone() const
{
    return const_cast<ICloneable*>(this)->Clone();
}

}