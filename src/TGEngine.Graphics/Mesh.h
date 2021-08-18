#pragma once

#include <memory>

#include "VertexBuffer.h"
#include "IndexBuffer.h"

TG_NAMESPACE_BEGIN

class Mesh
{
public:
    Mesh() = default;
    Mesh(const std::shared_ptr<VertexBuffer>& vertexBuffer, const std::shared_ptr<IndexBuffer>& indexBuffer);

public:
    void Use();
    void SetVertexBuffer(const std::shared_ptr<VertexBuffer>& vertexBuffer);
    void SetIndexBuffer(const std::shared_ptr<IndexBuffer>& indexBuffer);
    std::shared_ptr<VertexBuffer> GetVertexBuffer() noexcept;
    std::shared_ptr<const VertexBuffer> GetVertexBuffer() const noexcept;
    std::shared_ptr<IndexBuffer> GetIndexBuffer() noexcept;
    std::shared_ptr<const IndexBuffer> GetIndexBuffer() const noexcept;

protected:
    std::shared_ptr<VertexBuffer> _vertexBuffer;
    std::shared_ptr<IndexBuffer> _indexBuffer;
};

}
