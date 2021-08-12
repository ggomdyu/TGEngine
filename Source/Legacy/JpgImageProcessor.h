/**
 * @file    JpgImageProcessor.h
 * @author  ggomdyu
 * @since   04/22/2018
 */

#pragma once
#include "ImageProcessor.h"

namespace tgon
{

class JpgImageProcessor final :
    public ImageProcessor
{
/* @section Method */
public:
    static bool IsExactFormat(const uint8_t* fileData, size_t fileDataBytes);
    bool Initialize(const gsl::span<const std::byte>& fileData) override;
};

inline bool JpgImageProcessor::IsExactFormat(const uint8_t* fileData, std::size_t fileDataBytes)
{
    if (fileDataBytes < 2)
    {
        return false;
    }

    return (fileData[0] == 0xFF && fileData[1] == 0xD8);
}

inline bool JpgImageProcessor::Initialize(const gsl::span<const std::byte>& fileData)
{
    if (JpgImageProcessor::IsExactFormat(fileData, fileDataBytes) == false)
    {
        return false;
    }
}

} /* namespace tgon */
