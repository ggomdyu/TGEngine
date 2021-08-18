#include "Core/Algorithm.h"

#include "OpenGLDebug.h"

#include "../Graphics.h"

#ifdef _MSC_VER
#pragma comment(lib, "OpenGL32.Lib")
#endif

TG_NAMESPACE_BEGIN
namespace
{

constexpr GLenum ConvertPrimitiveTypeToNative(PrimitiveType primitiveType) noexcept
{
    constexpr GLenum primitiveTable[] = {
        GL_POINTS,
        GL_LINES,
        GL_LINE_STRIP,
        GL_TRIANGLES,
        GL_TRIANGLE_STRIP,
        GL_TRIANGLE_FAN,
    };

    return primitiveTable[UnderlyingCast(primitiveType)];
}

constexpr GLenum ConvertBlendModeToNative(BlendMode blendMode) noexcept
{
    constexpr GLenum blendModeTable[] = {
        GL_ZERO,
        GL_ONE,
        GL_DST_COLOR,
        GL_SRC_COLOR,
        GL_ONE_MINUS_DST_COLOR,
        GL_SRC_ALPHA,
        GL_ONE_MINUS_SRC_COLOR,
        GL_DST_ALPHA,
        GL_ONE_MINUS_DST_ALPHA,
        GL_SRC_ALPHA_SATURATE,
        GL_ONE_MINUS_SRC_ALPHA,
    };

    return blendModeTable[UnderlyingCast(blendMode)];
}

constexpr GLenum ConvertFillModeToNative(FillMode fillMode) noexcept
{
    constexpr GLenum fillModeTable[] = {
        GL_POINT,
        GL_LINE,
        GL_FILL,
    };

    return fillModeTable[UnderlyingCast(fillMode)];
}

constexpr GLenum ConvertCullModeToNative(CullMode cullMode) noexcept
{
    constexpr GLenum cullModeTable[] = {
        0,
        GL_FRONT,
        GL_BACK,
    };

    return cullModeTable[static_cast<int>(cullMode)];
}

}

OpenGLGraphics::OpenGLGraphics(void* nativeWindow, const VideoMode& videoMode) :
    _context(nativeWindow, videoMode)
{
    TG_GL_ERROR_CHECK(glFrontFace(GL_CW));

    TG_GL_ERROR_CHECK(glGenVertexArrays(1, &_vertexArrayHandle));
    TG_GL_ERROR_CHECK(glBindVertexArray(_vertexArrayHandle));
}

OpenGLGraphics::OpenGLGraphics(OpenGLGraphics&& rhs) noexcept :
    _context(std::move(rhs._context)),
    _vertexArrayHandle(rhs._vertexArrayHandle)
{
    rhs._vertexArrayHandle = 0;
}

OpenGLGraphics::~OpenGLGraphics()
{
    TG_GL_ERROR_CHECK(glBindVertexArray(_vertexArrayHandle));
    TG_GL_ERROR_CHECK(glDeleteVertexArrays(1, &_vertexArrayHandle));
}

OpenGLGraphics& OpenGLGraphics::operator=(OpenGLGraphics&& rhs) noexcept
{
    std::swap(_context, rhs._context);
    std::swap(_vertexArrayHandle, rhs._vertexArrayHandle);

    return *this;
}

void Graphics::SetScissorRect(const Rect& scissorRect)
{
    TG_GL_ERROR_CHECK(glScissor(static_cast<GLint>(scissorRect.x), static_cast<GLint>(scissorRect.y), static_cast<GLint>(scissorRect.width), static_cast<GLint>(scissorRect.height)));
}

void Graphics::SetClearColor(const Color& color)
{
    TG_GL_ERROR_CHECK(glClearColor(color.r, color.g, color.b, color.a));
}

void Graphics::SetFillMode(FillMode fillMode)
{
    TG_GL_ERROR_CHECK(glPolygonMode(GL_FRONT_AND_BACK, ConvertFillModeToNative(fillMode)));
}

void Graphics::SetCullMode(CullMode cullMode)
{
    if (cullMode == CullMode::Off)
    {
        TG_GL_ERROR_CHECK(glDisable(GL_CULL_FACE));
    }
    else
    {
        TG_GL_ERROR_CHECK(glEnable(GL_CULL_FACE));
        TG_GL_ERROR_CHECK(glCullFace(ConvertCullModeToNative(cullMode)));
    }
}

void Graphics::SetViewport(int32_t x, int32_t y, int32_t width, int32_t height)
{
    TG_GL_ERROR_CHECK(glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLsizei>(width), static_cast<GLsizei>(height)));
}

void Graphics::SetBlendMode(BlendMode srcFactor, BlendMode destFactor)
{
    TG_GL_ERROR_CHECK(glBlendFunc(ConvertBlendModeToNative(srcFactor), ConvertBlendModeToNative(destFactor)));
}

void Graphics::SetEnableZWrite(bool enable)
{
    if (enable)
    {
        TG_GL_ERROR_CHECK(glDepthMask(GL_TRUE));
    }
    else
    {
        TG_GL_ERROR_CHECK(glDepthMask(GL_FALSE));
    }
}

void Graphics::SetEnableZTest(bool enable)
{
    if (enable)
    {
        TG_GL_ERROR_CHECK(glEnable(GL_DEPTH_TEST));
    }
    else
    {
        TG_GL_ERROR_CHECK(glDisable(GL_DEPTH_TEST));
    }
}

void Graphics::SetEnableBlend(bool enable)
{
    if (enable)
    {
        TG_GL_ERROR_CHECK(glEnable(GL_BLEND));
    }
    else
    {
        TG_GL_ERROR_CHECK(glDisable(GL_BLEND));
    }
}

void Graphics::SetEnableScissorTest(bool enable)
{
    if (enable)
    {
        TG_GL_ERROR_CHECK(glEnable(GL_SCISSOR_TEST));
    }
    else
    {
        TG_GL_ERROR_CHECK(glDisable(GL_SCISSOR_TEST));
    }
}

void Graphics::ClearColorBuffer()
{
    TG_GL_ERROR_CHECK(glClear(GL_COLOR_BUFFER_BIT));
}

void Graphics::ClearColorDepthBuffer()
{
    TG_GL_ERROR_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void Graphics::SwapBuffer()
{
    _context.SwapBuffer();
}

void Graphics::DrawPrimitives(PrimitiveType primitiveType, int32_t vertexStartOffset, int32_t vertexCount)
{
    TG_GL_ERROR_CHECK(glDrawArrays(ConvertPrimitiveTypeToNative(primitiveType), static_cast<int32_t>(vertexStartOffset), vertexCount));
}

void Graphics::DrawIndexedPrimitives(PrimitiveType primitiveType, int32_t indexCount)
{
    TG_GL_ERROR_CHECK(glDrawElements(ConvertPrimitiveTypeToNative(primitiveType), indexCount, GL_UNSIGNED_INT, nullptr));
}

}