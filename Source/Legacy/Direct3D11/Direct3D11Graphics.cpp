#include "PrecompiledHeader.pch"

#include "D3D11Graphics.h"

#include "../Generic/GenericGraphicsType.h"

#include "Core/Debug/Log.h"
#include "Core/Platform/Window.h"

#pragma comment(lib, "dxgi.lib")
#pragma comment(lib, "d3d11.lib")
#pragma comment(lib, "d3dx11.lib")
#pragma comment(lib, "d3dx10.lib")

namespace tgon
{

D3D11Graphics::D3D11Graphics(const std::shared_ptr<Window>& window, const VideoMode& videoMode) :
    m_presentSyncInterval(videoMode.enableVerticalSync ? 1 : 0),
    m_clearColor{videoMode.clearColor.r, videoMode.clearColor.g, videoMode.clearColor.b, videoMode.clearColor.a}
{
    this->Initialize(window, videoMode);
}

D3D11Graphics::~D3D11Graphics()
{
    // 종료하기 전에 이렇게 윈도우 모드로 바꾸지 않으면 스왑체인을 할당 해제할 때 예외가 발생합니다.
    /*if (m_swapChain)
    {
        m_swapChain->SetFullscreenState(false, NULL);
    }*/
}

bool D3D11Graphics::Initialize(const std::shared_ptr<Window>& window, const VideoMode& videoMode)
{
    if (this->InitializeD3D(window, videoMode) == false)
    {
        return false;
    }

    return true;
}

bool D3D11Graphics::InitializeD3D(const std::shared_ptr<Window>& window, const VideoMode& videoMode)
{
    ComPtr<IDXGIFactory*> dxgiFactory;
    ComPtr<IDXGIAdapter*> dxgiAdapter;
    ComPtr<IDXGIOutput*> dxgiAdapterOutput;
    if (this->CreateDXGIObjects(&dxgiFactory, &dxgiAdapter, &dxgiAdapterOutput) == false)
    {
        return false;
    }

    // Create a device and swap chain
    {
        DXGI_MODE_DESC suitDisplayMode;
        if (this->FindSuitDisplayMode(dxgiAdapterOutput, DXGI_FORMAT::DXGI_FORMAT_R8G8B8A8_UNORM, &suitDisplayMode) == false)
        {
            return false;
        }

        this->CreateDeviceAndSwapChain(window, videoMode, suitDisplayMode, &m_device, &m_deviceContext, &m_swapChain);
    }

    // Create a render-target view for accessing resource data.
    if (this->CreateRenderTargetView(m_device, m_swapChain, &m_renderTargetView) == false)
    {
        return false;
    }

    // Register a depth stencil state to the context.
    {
        if (this->CreateDepthStencilBuffer(m_device, &m_depthStencilBuffer) == false)
        {
            return false;
        }
        
        if (this->CreateDepthStencilState(m_device, &m_depthStencilState) == false)
        {
            return false;
        }

        m_deviceContext->OMSetDepthStencilState(m_depthStencilState, 1);
    }

    // Now we will make depth stencil view. We must create it because Direct3D appreciate the depth stencil buffer is texture after do this.
    {
        if (this->CreateDepthStencilView(m_device, m_depthStencilBuffer, &m_depthStencilView) == false)
        {
            return false;
        }

        m_deviceContext->OMSetRenderTargets(1, &m_renderTargetView, m_depthStencilView);
    }

    // Register a rasterizer state to the context.
    {
        if (this->CreateRasterizerState(m_device, &m_rasterizerState) == false)
        {
            return false;
        }

        m_deviceContext->RSSetState(m_rasterizerState);
    }

    // Finally, Set the viewport.
    {
        int32_t windowWidth, windowHeight;
        window->GetSize(&windowWidth, &windowHeight);
        this->SetViewport(m_deviceContext, windowWidth, windowHeight, 0.0f, 1.0f, 0.0f, 0.0f);
    }

    return true;
}

bool D3D11Graphics::CreateDXGIObjects(IDXGIFactory** dxgiFactory, IDXGIAdapter** dxgiAdapter, IDXGIOutput** dxgiAdapterOutput)
{
    // Create a DXGI factory that you can use to generate other DXGI objects.
    if (FAILED(CreateDXGIFactory(__uuidof(IDXGIFactory), reinterpret_cast<void**>(dxgiFactory))))
    {
        Log("Failed to invoke CreateDXGIFactory.");
        return false;
    }

    // Enumerate the video cards.
    if (FAILED((*dxgiFactory)->EnumAdapters(0, dxgiAdapter)))
    {
        Log("Failed to invoke IDXGIFactory::EnumAdapters.");
        return false;
    }

    // Enumerate adapter outputs.
    if (FAILED((*dxgiAdapter)->EnumOutputs(0, dxgiAdapterOutput)))
    {
        Log("Failed to invoke IDXGIAdapter::EnumOutputs.");
        return false;
    }

    return true;
}

bool D3D11Graphics::EnumerateDisplayModes(IDXGIOutput* dxgiAdapterOutput, DXGI_FORMAT enumFormat, std::vector<DXGI_MODE_DESC>* displayModes)
{
    // Get the number of display modes which suitable for R8G8B8A8 format.
    UINT numDisplayModes;
    if (FAILED(dxgiAdapterOutput->GetDisplayModeList(enumFormat, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, nullptr)))
    {
        Log("Failed to invoke IDXGIOutput::GetDisplayModeList.");
        return false;
    }

    if (numDisplayModes > 0)
    {
        // Then, list those display modes to displayModes.
        displayModes->resize(numDisplayModes);
        if (FAILED(dxgiAdapterOutput->GetDisplayModeList(enumFormat, DXGI_ENUM_MODES_INTERLACED, &numDisplayModes, displayModes->data())))
        {
            Log("Failed to invoke IDXGIOutput::GetDisplayModeList.");
            return false;
        }
    }

    return true;
}

bool D3D11Graphics::FindSuitDisplayMode(IDXGIOutput* dxgiAdapterOutput, DXGI_FORMAT requiredFormat, DXGI_MODE_DESC* suitDisplayMode)
{
    std::vector<DXGI_MODE_DESC> displayModes;
    if (this->EnumerateDisplayModes(dxgiAdapterOutput, requiredFormat, &displayModes) == false)
    {
        return false;
    }
    else if (displayModes.size() <= 0)
    {
        return false;
    }

    for (const auto& displayMode : displayModes)
    {
        *suitDisplayMode = displayMode;
        break;
        //// TODO: 적절한 DisplayMode를 찾는 정책을 정해야 함
        //if (displayMode.Width == static_cast<UINT>(windowWidth) &&
        //    displayMode.Height == static_cast<UINT>(windowHeight))
        //{
        //    suitDisplayMode = displayMode;
        //    break;
        //}
    }

    return true;
}

bool D3D11Graphics::CreateDeviceAndSwapChain(const std::shared_ptr<Window>& window, const VideoMode& videoMode, const DXGI_MODE_DESC& suitDisplayMode, ID3D11Device** device, ID3D11DeviceContext** deviceContext, IDXGISwapChain** swapChain)
{
    auto windowSize = window->GetSize();

    DXGI_SWAP_CHAIN_DESC swapChainDesc {};
    swapChainDesc.OutputWindow = reinterpret_cast<HWND>(window->GetNativeWindow());
    swapChainDesc.Windowed = true;
    swapChainDesc.BufferCount = 1;
    swapChainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
    swapChainDesc.BufferDesc.Width = windowSize.width;
    swapChainDesc.BufferDesc.Height = windowSize.height;
    swapChainDesc.BufferDesc.Format = DXGI_FORMAT_R8G8B8A8_UNORM;
    swapChainDesc.BufferDesc.Scaling = DXGI_MODE_SCALING_UNSPECIFIED;
    swapChainDesc.BufferDesc.ScanlineOrdering = DXGI_MODE_SCANLINE_ORDER_UNSPECIFIED;
    swapChainDesc.SwapEffect = DXGI_SWAP_EFFECT_DISCARD; // Discard back buffer after drawing

    if (videoMode.enableVerticalSync)
    {
        swapChainDesc.BufferDesc.RefreshRate = suitDisplayMode.RefreshRate;
    }
    else
    {
        swapChainDesc.BufferDesc.RefreshRate.Numerator = 0;
        swapChainDesc.BufferDesc.RefreshRate.Denominator = 1;
    }

    if (videoMode.enableMultiSampling)
    {
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
        /*swapChainDesc.SampleDesc.Count = videoMode.sampleCount;
        swapChainDesc.SampleDesc.Quality = 100;*/
    }
    else
    {
        swapChainDesc.SampleDesc.Count = 1;
        swapChainDesc.SampleDesc.Quality = 0;
    }

    // We're going to use DirectX version 11.
    D3D_FEATURE_LEVEL featureLevel = D3D_FEATURE_LEVEL::D3D_FEATURE_LEVEL_11_0;
    D3D_DRIVER_TYPE driverType = (videoMode.enableHardwareAccelerate) ? D3D_DRIVER_TYPE_HARDWARE : D3D_DRIVER_TYPE_SOFTWARE;
    if (FAILED(D3D11CreateDeviceAndSwapChain(nullptr, driverType, nullptr, 0, &featureLevel, 1, D3D11_SDK_VERSION, &swapChainDesc, swapChain, device, nullptr, deviceContext)))
    {
        Log("Failed to invoke D3D11CreateDeviceAndSwapChain.");
        return false;
    }

    return true;
}

bool D3D11Graphics::CreateRenderTargetView(ID3D11Device* device, IDXGISwapChain* swapChain, ID3D11RenderTargetView** renderTargetView)
{
    // Access the swap-chain's back buffer.
    ComPtr<ID3D11Texture2D*> backBuffer;
    if (FAILED(swapChain->GetBuffer(0, __uuidof(ID3D11Texture2D), reinterpret_cast<void**>(&backBuffer))))
    {
        Log("Failed to invoke IDXGISwapChain::GetBuffer.");
        return false;
    }

    // Create a render-target view for accessing resource data.
    if (FAILED(device->CreateRenderTargetView(backBuffer, nullptr, renderTargetView)))
    {
        Log("Failed to invoke ID3D11Device::CreateRenderTargetView.");
        return false;
    }

    return true;
}

bool D3D11Graphics::CreateDepthStencilBuffer(ID3D11Device* device, ID3D11Texture2D** depthStencilBuffer)
{
    D3D11_TEXTURE2D_DESC depthBufferDesc {};
    depthBufferDesc.Width = 600;
    depthBufferDesc.Height = 600;
    depthBufferDesc.MipLevels = 1;
    depthBufferDesc.ArraySize = 1;
    depthBufferDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthBufferDesc.SampleDesc.Count = 1;
    depthBufferDesc.SampleDesc.Quality = 0;
    depthBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    depthBufferDesc.BindFlags = D3D11_BIND_DEPTH_STENCIL;
    depthBufferDesc.CPUAccessFlags = 0;
    depthBufferDesc.MiscFlags = 0;

    if (FAILED(device->CreateTexture2D(&depthBufferDesc, nullptr, depthStencilBuffer)))
    {
        Log("Failed to invoke ID3D11Device::CreateTexture2D.");
        return false;
    }

    return true;
}

bool D3D11Graphics::CreateDepthStencilState(ID3D11Device* device, ID3D11DepthStencilState** depthStencilState)
{
    D3D11_DEPTH_STENCIL_DESC depthStencilDesc {};
    depthStencilDesc.DepthEnable = true;
    depthStencilDesc.DepthWriteMask = D3D11_DEPTH_WRITE_MASK_ALL;
    depthStencilDesc.DepthFunc = D3D11_COMPARISON_LESS;
    depthStencilDesc.StencilEnable = true;
    depthStencilDesc.StencilReadMask = 0xff;
    depthStencilDesc.StencilWriteMask = 0xff;

    // Stencil operations if pixel is front-facing.
    depthStencilDesc.FrontFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilDepthFailOp = D3D11_STENCIL_OP_INCR;
    depthStencilDesc.FrontFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.FrontFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    // Stencil operations if pixel is back-facing.
    depthStencilDesc.BackFace.StencilFailOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilDepthFailOp = D3D11_STENCIL_OP_DECR;
    depthStencilDesc.BackFace.StencilPassOp = D3D11_STENCIL_OP_KEEP;
    depthStencilDesc.BackFace.StencilFunc = D3D11_COMPARISON_ALWAYS;

    if (FAILED(device->CreateDepthStencilState(&depthStencilDesc, depthStencilState)))
    {
        Log("Failed to invoke ID3D11Device::CreateTexture2D.");
        return false;
    }

    return true;
}

bool D3D11Graphics::CreateDepthStencilView(ID3D11Device* device, ID3D11Texture2D* depthStencilBuffer, ID3D11DepthStencilView** depthStencilView)
{
    D3D11_DEPTH_STENCIL_VIEW_DESC depthStencilViewDesc {};
    depthStencilViewDesc.Format = DXGI_FORMAT_D24_UNORM_S8_UINT;
    depthStencilViewDesc.ViewDimension = D3D11_DSV_DIMENSION_TEXTURE2D;
    depthStencilViewDesc.Texture2D.MipSlice = 0;

    if (FAILED(m_device->CreateDepthStencilView(depthStencilBuffer, &depthStencilViewDesc, depthStencilView)))
    {
        Log("Failed to invoke ID3D11Device::CreateDepthStencilView.");
        return false;
    }

    return true;
}

bool D3D11Graphics::CreateRasterizerState(ID3D11Device* device, ID3D11RasterizerState** rasterizerState)
{
    D3D11_RASTERIZER_DESC rasterizerDesc {};
    rasterizerDesc.AntialiasedLineEnable = false;
    rasterizerDesc.CullMode = D3D11_CULL_MODE::D3D11_CULL_BACK;
    rasterizerDesc.DepthBias = 0;
    rasterizerDesc.DepthBiasClamp = 0.0f;
    rasterizerDesc.DepthClipEnable = TRUE;
    rasterizerDesc.FillMode = D3D11_FILL_MODE::D3D11_FILL_SOLID;
    rasterizerDesc.FrontCounterClockwise = false;
    rasterizerDesc.MultisampleEnable = false;
    rasterizerDesc.ScissorEnable = false;
    rasterizerDesc.SlopeScaledDepthBias = 0.0;

    if (FAILED(m_device->CreateRasterizerState(&rasterizerDesc, rasterizerState)))
    {
        Log("Failed to invoke ID3D11Device::CreateRasterizerState.");
        return false;
    }

    return true;
}

void D3D11Graphics::SetViewport(ID3D11DeviceContext* deviceContext, int32_t width, int32_t height, float minDepth, float maxDepth, float topLeftX, float topLeftY)
{
    D3D11_VIEWPORT viewportDesc {};
    viewportDesc.Width = width;
    viewportDesc.Height = height;
    viewportDesc.MinDepth = minDepth;
    viewportDesc.MaxDepth = maxDepth;
    viewportDesc.TopLeftX = topLeftX;
    viewportDesc.TopLeftY = topLeftY;

    deviceContext->RSSetViewports(1, &viewportDesc);
}

void D3D11Graphics::SetClearColor(const Color4f& color)
{
    m_clearColor[0] = color.r;
    m_clearColor[1] = color.g;
    m_clearColor[2] = color.b;
    m_clearColor[3] = color.a;
}

void D3D11Graphics::SetFillMode(FillMode fillMode)
{
}

void D3D11Graphics::SetCullMode(CullMode cullMode)
{
}

void D3D11Graphics::EnableBlend()
{
}

void D3D11Graphics::EnableDepthTest()
{
}

void D3D11Graphics::DisableBlend()
{
}

void D3D11Graphics::DisableDepthTest()
{
}

void D3D11Graphics::ClearColorBuffer()
{
    m_deviceContext->ClearRenderTargetView(m_renderTargetView, m_clearColor);
}

void D3D11Graphics::ClearColorDepthBuffer()
{
    m_deviceContext->ClearRenderTargetView(m_renderTargetView, m_clearColor);
    m_deviceContext->ClearDepthStencilView(m_depthStencilView, D3D10_CLEAR_DEPTH, 1.0f, 0);
}

void D3D11Graphics::SwapBuffer()
{
    m_swapChain->Present(m_presentSyncInterval, 0);
}

//VertexBuffer D3D11Graphics::CreateVertexBuffer(const void* data, std::size_t dataBytes, bool isDynamicUsage, const std::initializer_list<VertexBufferDesc>& vertexBufferDescs)
//{
//    return VertexBuffer();
//}

} /* namespace tgon */