#pragma once

#include <vector>
#include <span>

#if TG_GRAPHICS_OPENGL
#include "OpenGL/OpenGLVertexBuffer.h"
#endif

TG_NAMESPACE_BEGIN

enum class VertexFormat
{
    Float,
    Double,
    Byte,
    UnsignedByte,
    Short,
    UnsignedShort,
    Int,
    UnsignedInt,
};

enum class VertexAttributeIndex
{
    Position = 0,
    Color = 1,
    UV = 2,
    Normal = 3,
};

struct VertexBufferLayout
{
    VertexAttributeIndex attribute;
    int32_t dimension;
    VertexFormat type;
    bool normalized;
    int32_t stride;
    std::size_t offset;
};

class VertexBuffer final :
    private PlatformVertexBuffer
{
public:
    using PlatformVertexBuffer::PlatformVertexBuffer;
    VertexBuffer(const VertexBuffer& rhs) = delete;
    VertexBuffer(VertexBuffer&& rhs) noexcept = default;
    ~VertexBuffer();

public:
    VertexBuffer& operator=(const VertexBuffer& rhs) = delete;
    VertexBuffer& operator=(VertexBuffer&& rhs) noexcept = default;

public:
    PlatformVertexBuffer& GetPlatformDependency() noexcept;
    const PlatformVertexBuffer& GetPlatformDependency() const noexcept;
    static std::optional<VertexBuffer> Create(const std::initializer_list<VertexBufferLayout>& vertexBufferLayouts = {});
    template <typename _Type>
    void SetData(const std::span<_Type>& data, bool isDynamicUsage);
    void SetData(const void* data, std::size_t dataByteCount, bool isDynamicUsage);
    void SetLayoutDescriptor(const std::initializer_list<VertexBufferLayout>& vertexBufferLayouts);
    void Use();
    void Disuse();

private:
    std::vector<VertexBufferLayout> _vertexBufferLayouts;
};

template <typename _Type>
void VertexBuffer::SetData(const std::span<_Type>& data, bool isDynamicUsage)
{
    this->SetData(data.data(), data.size() * sizeof(_Type), isDynamicUsage);
}

}
