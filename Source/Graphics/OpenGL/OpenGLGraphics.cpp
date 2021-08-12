#include "PrecompiledHeader.h"

#include "Core/Algorithm.h"

#include "OpenGLDebug.h"

#include "../Graphics.h"

#ifdef _MSC_VER
#pragma comment(lib, "OpenGL32.Lib")
#endif

namespace tg
{
namespace
{

[[nodiscard]] constexpr GLenum ConvertPrimitiveTypeToNative(PrimitiveType primitiveType) noexcept
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

[[nodiscard]] constexpr GLenum ConvertBlendModeToNative(BlendMode blendMode) noexcept
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

[[nodiscard]] constexpr GLenum ConvertFillModeToNative(FillMode fillMode) noexcept
{
    constexpr GLenum fillModeTable[] = {
        GL_POINT,
        GL_LINE,
        GL_FILL,
    };

    return fillModeTable[UnderlyingCast(fillMode)];
}

[[nodiscard]] constexpr GLenum ConvertCullModeToNative(CullMode cullMode) noexcept
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
    m_context(nativeWindow, videoMode)
{
    TGON_GL_ERROR_CHECK(glFrontFace(GL_CW));

    TGON_GL_ERROR_CHECK(glGenVertexArrays(1, &m_vertexArrayHandle));
    TGON_GL_ERROR_CHECK(glBindVertexArray(m_vertexArrayHandle));
}

OpenGLGraphics::OpenGLGraphics(OpenGLGraphics&& rhs) noexcept :
    m_context(std::move(rhs.m_context)),
    m_vertexArrayHandle(rhs.m_vertexArrayHandle)
{
    rhs.m_vertexArrayHandle = 0;
}

OpenGLGraphics::~OpenGLGraphics()
{
    TGON_GL_ERROR_CHECK(glBindVertexArray(m_vertexArrayHandle));
    TGON_GL_ERROR_CHECK(glDeleteVertexArrays(1, &m_vertexArrayHandle));
}

OpenGLGraphics& OpenGLGraphics::operator=(OpenGLGraphics&& rhs) noexcept
{
    std::swap(m_context, rhs.m_context);
    std::swap(m_vertexArrayHandle, rhs.m_vertexArrayHandle);

    return *this;
}

void Graphics::SetScissorRect(const Rect& scissorRect)
{
    TGON_GL_ERROR_CHECK(glScissor(static_cast<GLint>(scissorRect.x), static_cast<GLint>(scissorRect.y), static_cast<GLint>(scissorRect.width), static_cast<GLint>(scissorRect.height)));
}

void Graphics::SetClearColor(const Color& color)
{
    TGON_GL_ERROR_CHECK(glClearColor(color.r, color.g, color.b, color.a));
}

void Graphics::SetFillMode(FillMode fillMode)
{
    TGON_GL_ERROR_CHECK(glPolygonMode(GL_FRONT_AND_BACK, ConvertFillModeToNative(fillMode)));
}

void Graphics::SetCullMode(CullMode cullMode)
{
    if (cullMode == CullMode::Off)
    {
        TGON_GL_ERROR_CHECK(glDisable(GL_CULL_FACE));
    }
    else
    {
        TGON_GL_ERROR_CHECK(glEnable(GL_CULL_FACE));
        TGON_GL_ERROR_CHECK(glCullFace(ConvertCullModeToNative(cullMode)));
    }
}

void Graphics::SetViewport(int32_t x, int32_t y, int32_t width, int32_t height)
{
    TGON_GL_ERROR_CHECK(glViewport(static_cast<GLint>(x), static_cast<GLint>(y), static_cast<GLsizei>(width), static_cast<GLsizei>(height)));
}

void Graphics::SetBlendMode(BlendMode srcFactor, BlendMode destFactor)
{
    TGON_GL_ERROR_CHECK(glBlendFunc(ConvertBlendModeToNative(srcFactor), ConvertBlendModeToNative(destFactor)));
}

void Graphics::SetEnableZWrite(bool enable)
{
    if (enable)
    {
        TGON_GL_ERROR_CHECK(glDepthMask(GL_TRUE));
    }
    else
    {
        TGON_GL_ERROR_CHECK(glDepthMask(GL_FALSE));
    }
}

void Graphics::SetEnableZTest(bool enable)
{
    if (enable)
    {
        TGON_GL_ERROR_CHECK(glEnable(GL_DEPTH_TEST));
    }
    else
    {
        TGON_GL_ERROR_CHECK(glDisable(GL_DEPTH_TEST));
    }
}

void Graphics::SetEnableBlend(bool enable)
{
    if (enable)
    {
        TGON_GL_ERROR_CHECK(glEnable(GL_BLEND));
    }
    else
    {
        TGON_GL_ERROR_CHECK(glDisable(GL_BLEND));
    }
}

void Graphics::SetEnableScissorTest(bool enable)
{
    if (enable)
    {
        TGON_GL_ERROR_CHECK(glEnable(GL_SCISSOR_TEST));
    }
    else
    {
        TGON_GL_ERROR_CHECK(glDisable(GL_SCISSOR_TEST));
    }
}

void Graphics::ClearColorBuffer()
{
    TGON_GL_ERROR_CHECK(glClear(GL_COLOR_BUFFER_BIT));
}

void Graphics::ClearColorDepthBuffer()
{
    TGON_GL_ERROR_CHECK(glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT));
}

void Graphics::SwapBuffer()
{
    m_context.SwapBuffer();
}

void Graphics::DrawPrimitives(PrimitiveType primitiveType, int32_t vertexStartOffset, int32_t vertexCount)
{
    TGON_GL_ERROR_CHECK(glDrawArrays(ConvertPrimitiveTypeToNative(primitiveType), static_cast<int32_t>(vertexStartOffset), vertexCount));
}

void Graphics::DrawIndexedPrimitives(PrimitiveType primitiveType, int32_t indexCount)
{
    TGON_GL_ERROR_CHECK(glDrawElements(ConvertPrimitiveTypeToNative(primitiveType), indexCount, GL_UNSIGNED_INT, nullptr));
}

}