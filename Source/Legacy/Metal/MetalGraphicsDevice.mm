#import "PrecompiledHeader.pch"
#import "MetalGraphicsDevice.h"

namespace tgon
{
namespace graphics
{

MetalGraphicsDevice::MetalGraphicsDevice() :
    m_metalDevice(MTLCreateSystemDefaultDevice()),
    m_defaultLibrary([m_metalDevice newDefaultLibrary])
{
}

id<MTLCommandQueue> MetalGraphicsDevice::CreateCommandQueue() const
{
    return [m_metalDevice newCommandQueue];
}

id<MTLBuffer> MetalGraphicsDevice::CreateBuffer(uint32_t bufferSize) const
{
    return [m_metalDevice newBufferWithLength:bufferSize options:MTLResourceStorageModeShared];
}

id<MTLDepthStencilState> MetalGraphicsDevice::CreateDepthStencilState(MTLDepthStencilDescriptor* depthStencilDesc) const
{
    return [m_metalDevice newDepthStencilStateWithDescriptor:depthStencilDesc];
}

id<MTLRenderPipelineState> MetalGraphicsDevice::CreateGraphicsPipeline(MTLRenderPipelineDescriptor* renderPipelineDesc, NSError** error) const
{
    return [m_metalDevice newRenderPipelineStateWithDescriptor:renderPipelineDesc error:error];
}

id<MTLDevice> MetalGraphicsDevice::GetMetalDevice()
{
    return m_metalDevice;
}

id<MTLLibrary> MetalGraphicsDevice::GetDefaultLibrary()
{
    return m_defaultLibrary;
}

}
}
