#include "Mesh.h"

TG_NAMESPACE_BEGIN

Mesh::Mesh(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer) :
    _vertexBuffer(vertexBuffer),
    _indexBuffer(indexBuffer)
{
}

void Mesh::Use()
{
    if (_vertexBuffer != nullptr)
    {
        _vertexBuffer->Use();
    }

    if (_indexBuffer != nullptr)
    {
        _indexBuffer->Use();
    }
}

void Mesh::SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer)
{
    _vertexBuffer = vertexBuffer;
}

void Mesh::SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer)
{
    _indexBuffer = indexBuffer;
}

std::shared_ptr<VertexBuffer> Mesh::GetVertexBuffer() noexcept
{
    return _vertexBuffer;
}
    
std::shared_ptr<const VertexBuffer> Mesh::GetVertexBuffer() const noexcept
{
    return _vertexBuffer;
}

std::shared_ptr<IndexBuffer> Mesh::GetIndexBuffer() noexcept
{
    return _indexBuffer;
}

std::shared_ptr<const IndexBuffer> Mesh::GetIndexBuffer() const noexcept
{
    return _indexBuffer;
}

}
