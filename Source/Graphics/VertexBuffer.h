#pragma once

#include <vector>
#include <span>

#if TGON_GRAPHICS_OPENGL
#include "OpenGL/OpenGLVertexBuffer.h"
#endif

namespace tg
{

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
/**@section Constructor */
public:
    using PlatformVertexBuffer::PlatformVertexBuffer;
    VertexBuffer(const VertexBuffer& rhs) = delete;
    VertexBuffer(VertexBuffer&& rhs) noexcept = default;
    
/**@section Destructor */
public:
    ~VertexBuffer();

/**@section Operator */
public:
    VertexBuffer& operator=(const VertexBuffer& rhs) = delete;
    VertexBuffer& operator=(VertexBuffer&& rhs) noexcept = default;

/**@section Method */
public:
    [[nodiscard]] PlatformVertexBuffer& GetPlatformDependency() noexcept;
    [[nodiscard]] const PlatformVertexBuffer& GetPlatformDependency() const noexcept;
    [[nodiscard]] static std::optional<VertexBuffer> Create(const std::initializer_list<VertexBufferLayout>& vertexBufferLayouts = {});
    template <typename _Type>
    void SetData(const std::span<_Type>& data, bool isDynamicUsage);
    void SetData(const void* data, std::size_t dataByteCount, bool isDynamicUsage);
    void SetLayoutDescriptor(const std::initializer_list<VertexBufferLayout>& vertexBufferLayouts);
    void Use();
    void Disuse();

/**@section Variable */
private:
    std::vector<VertexBufferLayout> m_vertexBufferLayouts;
};

template <typename _Type>
void VertexBuffer::SetData(const std::span<_Type>& data, bool isDynamicUsage)
{
    this->SetData(data.data(), data.size() * sizeof(_Type), isDynamicUsage);
}

}
