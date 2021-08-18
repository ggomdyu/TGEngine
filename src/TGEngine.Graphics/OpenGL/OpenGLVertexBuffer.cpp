#if TG_GRAPHICS_OPENGL
#include <cassert>

#include "OpenGLDebug.h"

#include "../VertexBuffer.h"

TG_NAMESPACE_BEGIN
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
    _vertexBufferId(vertexBufferHandle)
{
}

OpenGLVertexBuffer::OpenGLVertexBuffer(OpenGLVertexBuffer&& rhs) noexcept :
    _vertexBufferId(rhs._vertexBufferId)
{
    rhs._vertexBufferId = 0;
}

OpenGLVertexBuffer& OpenGLVertexBuffer::operator=(OpenGLVertexBuffer&& rhs) noexcept
{
    std::swap(_vertexBufferId, rhs._vertexBufferId);
    return *this;
}

GLuint OpenGLVertexBuffer::CreateVertexBufferId()
{
    GLuint vertexBufferId;
    TG_GL_ERROR_CHECK(glGenBuffers(1, &vertexBufferId));

    return vertexBufferId;
}

GLuint OpenGLVertexBuffer::GetVertexBufferHandle() const noexcept
{
    return _vertexBufferId;
}

VertexBuffer::~VertexBuffer()
{
    TG_GL_ERROR_CHECK(glDeleteBuffers(1, &_vertexBufferId));
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
    TG_GL_ERROR_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId));
    TG_GL_ERROR_CHECK(glBufferData(GL_ARRAY_BUFFER, dataByteCount, data, isDynamicUsage ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
}

void VertexBuffer::SetLayoutDescriptor(const std::initializer_list<VertexBufferLayout>& vertexBufferLayouts)
{
    _vertexBufferLayouts = vertexBufferLayouts;
}

void VertexBuffer::Use()
{
    TG_GL_ERROR_CHECK(glBindBuffer(GL_ARRAY_BUFFER, _vertexBufferId));

    for (size_t i = 0; i < _vertexBufferLayouts.size(); ++i)
    {
        const auto& vertexBufferLayout = _vertexBufferLayouts[i];

        TG_GL_ERROR_CHECK(glEnableVertexAttribArray(GLuint(i)));
        TG_GL_ERROR_CHECK(glVertexAttribPointer(
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
    for (GLuint i = 0; i < _vertexBufferLayouts.size(); ++i)
    {
        TG_GL_ERROR_CHECK(glDisableVertexAttribArray(i));
    }
}

}
#endif
