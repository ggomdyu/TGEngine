#pragma once

#include <span>

#if TG_GRAPHICS_OPENGL
#include "OpenGL/OpenGLIndexBuffer.h"
#endif

TG_NAMESPACE_BEGIN
    
class IndexBuffer :
    private PlatformIndexBuffer
{
public:
    IndexBuffer();
    IndexBuffer(const IndexBuffer& rhs) = delete;
    IndexBuffer(IndexBuffer&& rhs) noexcept = default;
    ~IndexBuffer();
    
public:
    IndexBuffer& operator=(const IndexBuffer& rhs) = delete;
    IndexBuffer& operator=(IndexBuffer&& rhs) noexcept = default;

public:
    PlatformIndexBuffer& GetPlatformDependency() noexcept;
    const PlatformIndexBuffer& GetPlatformDependency() const noexcept;
    template <typename _Type>
    void SetData(const std::span<_Type>& data, bool isDynamicUsage);
    void SetData(const void* data, int32_t dataBytes, bool isDynamicUsage);
    void Use();
    void Unuse();
};

template <typename _Type>
inline void IndexBuffer::SetData(const std::span<_Type>& data, bool isDynamicUsage)
{
    this->SetData(data.data(), data.size() * sizeof(_Type), isDynamicUsage);
}

}
