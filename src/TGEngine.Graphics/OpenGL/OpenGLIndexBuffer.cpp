#include <cassert>

#include "OpenGLDebug.h"

#include "../IndexBuffer.h"

TG_NAMESPACE_BEGIN

OpenGLIndexBuffer::OpenGLIndexBuffer(GLuint indexBufferHandle) noexcept :
    _indexBufferHandle(indexBufferHandle)
{
}

OpenGLIndexBuffer::OpenGLIndexBuffer(OpenGLIndexBuffer&& rhs) noexcept :
    _indexBufferHandle(rhs._indexBufferHandle)
{
    rhs._indexBufferHandle = 0;
}

OpenGLIndexBuffer& OpenGLIndexBuffer::operator=(OpenGLIndexBuffer&& rhs) noexcept
{
    std::swap(_indexBufferHandle, rhs._indexBufferHandle);
    return *this;
}

GLuint OpenGLIndexBuffer::CreateIndexBufferId()
{
    GLuint indexBufferId;
    TG_GL_ERROR_CHECK(glGenBuffers(1, &indexBufferId));

    return indexBufferId;
}

GLuint OpenGLIndexBuffer::GetIndexBufferHandle() const noexcept
{
    return _indexBufferHandle;
}

IndexBuffer::IndexBuffer() :
    OpenGLIndexBuffer(CreateIndexBufferId())
{
}

IndexBuffer::~IndexBuffer()
{
    TG_GL_ERROR_CHECK(glDeleteBuffers(1, &_indexBufferHandle));
    _indexBufferHandle = 0;
}

void IndexBuffer::SetData(const void* data, int32_t dataBytes, bool isDynamicUsage)
{
    TG_GL_ERROR_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferHandle));
    TG_GL_ERROR_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataBytes, data, isDynamicUsage ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
}

void IndexBuffer::Use()
{
    TG_GL_ERROR_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, _indexBufferHandle));
}

void IndexBuffer::Unuse()
{
    TG_GL_ERROR_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

}
