/**
 * @filename    MetalBuffer.h
 * @author      ggomdyu
 * @since       07/03/2017
 */

#import <Metal/MTLDevice.h>
#import <Metal/MTLBuffer.h>

#import "GenericBuffer.h"

namespace tgon
{
namespace graphics
{

class MetalBuffer :
    public GenericBuffer<MetalBuffer>
{
/* @section Ctor/Dtor */
public:
    MetalBuffer(id<MTLDevice> device, const BufferCreateDescription& bufferCreateDesc);
    constexpr explicit MetalBuffer(id<MTLBuffer> buffer) noexcept;

/* @section Public method */
public:
    /* @brief Get buffer which is independent type. */
    void* GetBuffer();

    /* @brief Get buffer which is dependent type. */
    id<MTLBuffer> GetMetalBuffer();

    /* @brief Get allocated size of buffer. */
    uint32_t GetBufferSize() const;

/* @section Private variable */
private:
    id<MTLBuffer> m_metalBuffer;
};

} /* namespace graphics */
} /* namespace tgon */
