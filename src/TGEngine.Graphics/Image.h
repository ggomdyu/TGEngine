#pragma once

#include <memory>
#include <optional>

#include "Math/Extent.h"

TG_NAMESPACE_BEGIN

enum class ImageFormat
{
    Unknown = 0,
    Bmp,
    Emf,
    Exif,
    Gif,
    Icon,
    Jpeg,
    MemoryBmp,
    Png,
    Tiff,
    Wmf,
};

enum class RotateFlipType
{
    Rotate180FlipXY = 0,
    RotateNoneFlipNone = 0,
    Rotate270FlipXY = 1,
    Rotate90FlipNone = 1,
    Rotate180FlipNone = 2,
    RotateNoneFlipXY = 2,
    Rotate270FlipNone = 3,
    Rotate90FlipXY = 3,
    Rotate180FlipY = 4,
    RotateNoneFlipX = 4,
    Rotate270FlipY = 5,
    Rotate90FlipX = 5,
    Rotate180FlipX = 6,
    RotateNoneFlipY = 6,
    Rotate270FlipX = 7,
    Rotate90FlipY = 7,
};

enum class PixelFormat
{
    Unknown = 0,
    RGBA8888,
    RGB888,
    RGBA4444,
    R8,
};

class Image final
{
private:
    Image(std::unique_ptr<std::byte[]> imageData, const I32Extent2D& size, PixelFormat pixelFormat);

public:
    bool operator==(std::nullptr_t rhs) const noexcept;
    bool operator!=(std::nullptr_t rhs) const noexcept;
    std::byte& operator[](int32_t index);
    std::byte operator[](int32_t index) const;

public:
    static std::optional<Image> Create(const char8_t* filePath);
    static std::optional<Image> Create(const std::span<const std::byte>& bytes);
    std::byte* GetImageData() noexcept;
    const std::byte* GetImageData() const noexcept;
    int32_t GetWidth() const noexcept;
    int32_t GetHeight() const noexcept;
    const I32Extent2D& GetSize() const noexcept;
    PixelFormat GetPixelFormat() const noexcept;
    bool Save(const char8_t* filePath, ImageFormat format) const;
    void RotateFlip(RotateFlipType rotateFlipType);

private:
    std::unique_ptr<std::byte[]> _imageData;
    I32Extent2D _size;
    static constexpr PixelFormat InternalPixelFormat = PixelFormat::RGBA8888;
};

}
