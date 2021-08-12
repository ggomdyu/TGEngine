/**
 * @file    ImageProcessor.h
 * @author  ggomdyu
 * @since   01/20/2018
 */

#pragma once
#include <cstdint>
#include <memory>
#include <gsl/span>

namespace tgon
{

class ImageProcessor
{
/* @section Constructor */
public:
    ImageProcessor() noexcept = default;

/* @section Method */
public:
    virtual bool Initialize(const gsl::span<const std::byte>& fileData) = 0;
    std::byte* GetImageData() noexcept;
    const std::byte* GetImageData() const noexcept;
    int32_t GetWidth() const noexcept;
    int32_t GetHeight() const noexcept;
    int32_t GetColorDepth() const noexcept;
    int32_t GetChannels() const noexcept;

/* @section Variable */
protected:
    std::unique_ptr<std::byte[]> m_imageData;
    int32_t m_width = 0;
    int32_t m_height = 0;
    int32_t m_colorDepth = 0;
    int32_t m_channels = 0;
};

inline std::byte* ImageProcessor::GetImageData() noexcept
{
    return &m_imageData[0];
}

inline const std::byte* ImageProcessor::GetImageData() const noexcept
{
    return &m_imageData[0];
}

inline int32_t ImageProcessor::GetWidth() const noexcept
{
    return m_width;
}

inline int32_t ImageProcessor::GetHeight() const noexcept
{
    return m_height;
}

inline int32_t ImageProcessor::GetColorDepth() const noexcept
{
    return m_colorDepth;
}

inline int32_t ImageProcessor::GetChannels() const noexcept
{
    return m_channels;
}

} /* namespace tgon */  
