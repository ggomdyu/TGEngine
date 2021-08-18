#include "IndexBuffer.h"

TG_NAMESPACE_BEGIN

PlatformIndexBuffer& IndexBuffer::GetPlatformDependency() noexcept
{
    return *this;
}

const PlatformIndexBuffer& IndexBuffer::GetPlatformDependency() const noexcept
{
    return *this;
}

}
