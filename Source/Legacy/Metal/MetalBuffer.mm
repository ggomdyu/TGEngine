#import "MetalBuffer.h"

namespace tgon
{
namespace graphics
{

MetalBuffer::MetalBuffer(id<MTLDevice> device, const BufferCreateDescription& bufferCreateDesc) :
    m_metalBuffer([device newBufferWithLength:bufferCreateDesc.bufferSize options:MTLResourceStorageModeShared])
{
}

constexpr MetalBuffer::MetalBuffer(id<MTLBuffer> metalBuffer) noexcept :
    m_metalBuffer(metalBuffer)
{
}

void* MetalBuffer::GetBuffer()
{
    return [m_metalBuffer contents];
}

id<MTLBuffer> MetalBuffer::GetMetalBuffer()
{
    return m_metalBuffer;
}

uint32_t MetalBuffer::GetBufferSize() const
{
    return static_cast<uint32_t>([m_metalBuffer length]);
}

} /* namespace graphics */
} /* namespace tgon */
