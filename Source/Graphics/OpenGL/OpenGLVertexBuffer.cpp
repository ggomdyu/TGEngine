#include "PrecompiledHeader.h"

#if TGON_GRAPHICS_OPENGL
#include <cassert>

#include "OpenGLDebug.h"

#include "../VertexBuffer.h"

namespace tg
{
namespace
{

constexpr GLenum ConvertVertexFormatTypeToNative(VertexFormat vertexFormatType) noexcept
{
    constexpr GLenum nativeVertexFormatTypes[] = {
        GL_FLOAT,
        GL_DOUBLE,
        GL_BYTE,
        GL_UNSIGNED_BYTE,
        GL_SHORT,
        GL_UNSIGNED_SHORT,
        GL_INT,
        GL_UNSIGNED_INT
    };
    
    return nativeVertexFormatTypes[static_cast<int>(vertexFormatType)];
}

}

OpenGLVertexBuffer::OpenGLVertexBuffer(GLuint vertexBufferHandle) noexcept :
    m_vertexBufferId(vertexBufferHandle)
{
}

OpenGLVertexBuffer::OpenGLVertexBuffer(OpenGLVertexBuffer&& rhs) noexcept :
    m_vertexBufferId(rhs.m_vertexBufferId)
{
    rhs.m_vertexBufferId = 0;
}

OpenGLVertexBuffer& OpenGLVertexBuffer::operator=(OpenGLVertexBuffer&& rhs) noexcept
{
    std::swap(m_vertexBufferId, rhs.m_vertexBufferId);
    return *this;
}

GLuint OpenGLVertexBuffer::CreateVertexBufferId()
{
    GLuint vertexBufferId;
    TGON_GL_ERROR_CHECK(glGenBuffers(1, &vertexBufferId));

    return vertexBufferId;
}

GLuint OpenGLVertexBuffer::GetVertexBufferHandle() const noexcept
{
    return m_vertexBufferId;
}

VertexBuffer::~VertexBuffer()
{
    TGON_GL_ERROR_CHECK(glDeleteBuffers(1, &m_vertexBufferId));
}

std::optional<VertexBuffer> VertexBuffer::Create(const std::initializer_list<VertexBufferLayout>& vertexBufferLayouts)
{
    const auto vertexBufferId = CreateVertexBufferId();
    if (vertexBufferId == 0)
    {
        return {};
    }

    VertexBuffer vertexBuffer(vertexBufferId);
    vertexBuffer.SetLayoutDescriptor(vertexBufferLayouts);
    return std::move(vertexBuffer);
}

void VertexBuffer::SetData(const void* data, std::size_t dataByteCount, bool isDynamicUsage)
{
    TGON_GL_ERROR_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId));
    TGON_GL_ERROR_CHECK(glBufferData(GL_ARRAY_BUFFER, dataByteCount, data, isDynamicUsage ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
}

void VertexBuffer::SetLayoutDescriptor(const std::initializer_list<VertexBufferLayout>& vertexBufferLayouts)
{
    m_vertexBufferLayouts = vertexBufferLayouts;
}

void VertexBuffer::Use()
{
    TGON_GL_ERROR_CHECK(glBindBuffer(GL_ARRAY_BUFFER, m_vertexBufferId));

    for (size_t i = 0; i < m_vertexBufferLayouts.size(); ++i)
    {
        const auto& vertexBufferLayout = m_vertexBufferLayouts[i];

        TGON_GL_ERROR_CHECK(glEnableVertexAttribArray(GLuint(i)));
        TGON_GL_ERROR_CHECK(glVertexAttribPointer(
            static_cast<GLuint>(vertexBufferLayout.attribute),
            vertexBufferLayout.dimension,
            ConvertVertexFormatTypeToNative(vertexBufferLayout.type),
            vertexBufferLayout.normalized,
            vertexBufferLayout.stride,
            reinterpret_cast<const void*>(vertexBufferLayout.offset)
        ));
    }
}

void VertexBuffer::Disuse()
{
    for (GLuint i = 0; i < m_vertexBufferLayouts.size(); ++i)
    {
        TGON_GL_ERROR_CHECK(glDisableVertexAttribArray(i));
    }
}

}
#endif
