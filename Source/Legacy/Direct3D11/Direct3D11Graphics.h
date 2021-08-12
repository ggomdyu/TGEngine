/**
 * @filename    Direct3D11DynamicGraphics.h
 * @author      ggomdyu
 * @since       02/23/2018
 */

#pragma once
#include "D3D11GraphicsFwd.h"

#include "../Generic/GenericGraphics.h"

#include "Core/Utility/Windows/ComPtr.h"

#include <memory>
#include <vector>
#include <d3d11.h>

namespace tgon
{

class D3D11Graphics :
    public GenericGraphics
{
/* @section Public constructor */
public:
    D3D11Graphics(const std::shared_ptr<Window>& window, const VideoMode& videoMode);
    virtual ~D3D11Graphics() override;

/* @section Public method */
public:
    virtual void SetClearColor(const Color4f& color) override;
    virtual void SetFillMode(FillMode fillMode) final override;
    virtual void SetCullMode(CullMode cullMode) final override;
    virtual void SetScissorRect(const FRect& rect) final override {}
    virtual void EnableBlend() final override;
    virtual void EnableDepthTest() final override;
    virtual void DisableBlend() final override;
    virtual void DisableDepthTest() final override;
    virtual void ClearColorBuffer() final override;
    virtual void ClearColorDepthBuffer() final override;
    virtual void SwapBuffer() final override;
    //virtual VertexBuffer CreateVertexBuffer(const void* data, std::size_t dataBytes, bool isDynamicUsage, const std::initializer_list<VertexBufferDesc>& vertexBufferDescs) final override;

/* @section Private method */
private:
    bool Initialize(const std::shared_ptr<Window>& window, const VideoMode& videoMode);
    bool InitializeD3D(const std::shared_ptr<Window>& window, const VideoMode& videoMode);
    
    bool CreateDXGIObjects(IDXGIFactory** dxgiFactory, IDXGIAdapter** dxgiAdapter, IDXGIOutput** dxgiAdapterOutput);
    bool EnumerateDisplayModes(IDXGIOutput* dxgiAdapterOutput, DXGI_FORMAT enumFormat, std::vector<DXGI_MODE_DESC>* displayModes);
    bool FindSuitDisplayMode(IDXGIOutput* dxgiAdapterOutput, DXGI_FORMAT requiredFormat, DXGI_MODE_DESC* suitDisplayMode);
    bool CreateDeviceAndSwapChain(const std::shared_ptr<Window>& window, const VideoMode& videoMode, const DXGI_MODE_DESC& suitDispalyMode, ID3D11Device** device, ID3D11DeviceContext** deviceContext, IDXGISwapChain** m_swapChain);
    bool CreateRenderTargetView(ID3D11Device* device, IDXGISwapChain* swapChain, ID3D11RenderTargetView** renderTargetView);
    bool CreateDepthStencilBuffer(ID3D11Device* device, ID3D11Texture2D** depthStencilBuffer);
    bool CreateDepthStencilState(ID3D11Device* device, ID3D11DepthStencilState** depthStencilState);
    bool CreateDepthStencilView(ID3D11Device* device, ID3D11Texture2D* depthStencilBuffer, ID3D11DepthStencilView** depthStencilView);
    bool CreateRasterizerState(ID3D11Device* device, ID3D11RasterizerState** rasterizerState);
    
    void SetViewport(ID3D11DeviceContext* deviceContext, int32_t width, int32_t height, float minDepth, float maxDepth, float topLeftX, float topLeftY);

/* @section Private variable */
private:
    ComPtr<ID3D11Device*> m_device;
    ComPtr<ID3D11DeviceContext*> m_deviceContext;
    ComPtr<ID3D11RenderTargetView*> m_renderTargetView;
    ComPtr<IDXGISwapChain*> m_swapChain;
    ComPtr<ID3D11DepthStencilState*> m_depthStencilState;
    ComPtr<ID3D11Texture2D*> m_depthStencilBuffer;
    ComPtr<ID3D11DepthStencilView*> m_depthStencilView;
    ComPtr<ID3D11RasterizerState*> m_rasterizerState;

    FLOAT m_clearColor[4];
    UINT m_presentSyncInterval;
};

} /* namespace tgon */