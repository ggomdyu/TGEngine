#include "PrecompiledHeader.h"

#include <cassert>

#include "OpenGLDebug.h"

#include "../IndexBuffer.h"

namespace tg
{

OpenGLIndexBuffer::OpenGLIndexBuffer(GLuint indexBufferHandle) noexcept :
    m_indexBufferHandle(indexBufferHandle)
{
}

OpenGLIndexBuffer::OpenGLIndexBuffer(OpenGLIndexBuffer&& rhs) noexcept :
    m_indexBufferHandle(rhs.m_indexBufferHandle)
{
    rhs.m_indexBufferHandle = 0;
}

OpenGLIndexBuffer& OpenGLIndexBuffer::operator=(OpenGLIndexBuffer&& rhs) noexcept
{
    std::swap(m_indexBufferHandle, rhs.m_indexBufferHandle);
    return *this;
}

GLuint OpenGLIndexBuffer::CreateIndexBufferId()
{
    GLuint indexBufferId;
    TGON_GL_ERROR_CHECK(glGenBuffers(1, &indexBufferId));

    return indexBufferId;
}

GLuint OpenGLIndexBuffer::GetIndexBufferHandle() const noexcept
{
    return m_indexBufferHandle;
}

IndexBuffer::IndexBuffer() :
    OpenGLIndexBuffer(CreateIndexBufferId())
{
}

IndexBuffer::~IndexBuffer()
{
    TGON_GL_ERROR_CHECK(glDeleteBuffers(1, &m_indexBufferHandle));
    m_indexBufferHandle = 0;
}

void IndexBuffer::SetData(const void* data, int32_t dataBytes, bool isDynamicUsage)
{
    TGON_GL_ERROR_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle));
    TGON_GL_ERROR_CHECK(glBufferData(GL_ELEMENT_ARRAY_BUFFER, dataBytes, data, isDynamicUsage ? GL_DYNAMIC_DRAW : GL_STATIC_DRAW));
}

void IndexBuffer::Use()
{
    TGON_GL_ERROR_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_indexBufferHandle));
}

void IndexBuffer::Unuse()
{
    TGON_GL_ERROR_CHECK(glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0));
}

}
