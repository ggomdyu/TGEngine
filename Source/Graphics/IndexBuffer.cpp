#include "PrecompiledHeader.h"

#include "IndexBuffer.h"

namespace tg
{

PlatformIndexBuffer& IndexBuffer::GetPlatformDependency() noexcept
{
    return *this;
}

const PlatformIndexBuffer& IndexBuffer::GetPlatformDependency() const noexcept
{
    return *this;
}

}
