#include "VertexBuffer.h"

TG_NAMESPACE_BEGIN

PlatformVertexBuffer& VertexBuffer::GetPlatformDependency() noexcept
{
    return *this;
}

const PlatformVertexBuffer& VertexBuffer::GetPlatformDependency() const noexcept
{
    return *this;
}

}
