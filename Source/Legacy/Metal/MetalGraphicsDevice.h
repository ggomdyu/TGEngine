/**
 * @filename    MetalDevice.h
 * @author      ggomdyu
 * @since       07/02/2017
 */

#import <cstddef>
#import <Metal/MTLDevice.h>
#import <Metal/MTLLibrary.h>

#import "GraphicsCore/Graphics/Generic/GenericGraphicsDevice.h"

#import "MetalBuffer.h"

namespace tgon
{
namespace graphics
{

class MetalGraphicsDevice :
    public GenericGraphicsDevice<MetalGraphicsDevice>
{
/* @section Ctor/Dtor */
public:
    MetalGraphicsDevice();
    virtual ~MetalGraphicsDevice() = default;

/* @section Public method */
public:
    /* @brief   Create a command queue. */
    id<MTLCommandQueue> CreateCommandQueue() const;

    /* @brief   Create a buffer which will be used for specific purpose: Uniform-buffer, etc */
    id<MTLBuffer> CreateBuffer(uint32_t bufferSize) const;

    /* @brief   Create a Depth-stencil-state. */
    id<MTLDepthStencilState> CreateDepthStencilState(MTLDepthStencilDescriptor* depthStencilDesc) const;

    /**
     * @brief                           Create a graphics pipeline.
     * @param renderPipelineDesc [in]   Description to create graphics pipeline.
     * @param error [out]               The destination of error.
     */
    id<MTLRenderPipelineState> CreateGraphicsPipeline(MTLRenderPipelineDescriptor* renderPipelineDesc, NSError** error) const;

    id<MTLDevice> GetMetalDevice();
    
    id<MTLLibrary> GetDefaultLibrary();

/* @section Private variable */
private:
    id<MTLDevice> m_metalDevice;

    id<MTLLibrary> m_defaultLibrary;
};

}
}
