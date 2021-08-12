#pragma once

#include "Math/Color.h"
#include "Math/Rect.h"

#if TGON_GRAPHICS_OPENGL
#include "OpenGL/OpenGLGraphics.h"
#endif

namespace tg
{

enum class PrimitiveType
{
    Points,
    Lines,
    LineStrip,
    Triangles,
    TriangleStrip,
    TriangleFan,
};

enum class FillMode
{
    Point,
    WireFrame,
    Solid,
};

enum class CullMode
{
    Off,
    Front,
    Back,
};

enum class BlendMode
{
    Zero,
    One,
    DstColor,
    SrcColor,
    OneMinusDstColor,
    SrcAlpha,
    OneMinusSrcColor,
    DstAlpha,
    OneMinusDstAlpha,
    SrcAlphaSaturate,
    OneMinusSrcAlpha,
};

class Graphics final :
    private PlatformGraphics
{
/**@section Constructor */
public:
    Graphics(void* nativeWindow, const VideoMode& videoMode);

/**@section Method */
public:
    [[nodiscard]] PlatformGraphics& GetPlatformDependency() noexcept;
    [[nodiscard]] const PlatformGraphics& GetPlatformDependency() const noexcept;
    void SetScissorRect(const Rect& scissorRect);
    void SetClearColor(const Color& color);
    void SetFillMode(FillMode fillMode);
    void SetCullMode(CullMode cullMode);
    void SetViewport(int32_t x, int32_t y, int32_t width, int32_t height);
    void SetBlendMode(BlendMode srcFactor, BlendMode destFactor);
    void SetEnableZWrite(bool enable);
    void SetEnableZTest(bool enable);
    void SetEnableBlend(bool enable);
    void SetEnableScissorTest(bool enable);
    void ClearColorBuffer();
    void ClearColorDepthBuffer();
    void SwapBuffer();
    void DrawPrimitives(PrimitiveType primitiveType, int32_t vertexStartOffset, int32_t vertexCount);
    void DrawIndexedPrimitives(PrimitiveType primitiveType, int32_t indexCount);

/**@section Variable */
private:
    void* m_nativeWindow;
};

}
