/**
 * @file    WebPImageProcessor.h
 * @author  ggomdyu
 * @since   04/22/2018
 */

#pragma once
#include "ImageProcessor.h"

namespace tgon
{

class WebPImageProcessor :
    public ImageProcessor
{
/* @section Method */
public:
    static bool IsWebP(const gsl::span<const std::byte>& fileData);
    bool Initialize(const gsl::span<const std::byte>& fileData) override;
};

inline bool WebPImageProcessor::Initialize(const gsl::span<const std::byte>& fileData)
{
    if (WebPImageProcessor::CheckFormat(fileData) == false)
    {
        return false;
    }

    return false;
}

inline bool WebPImageProcessor::CheckFormat(const gsl::span<const std::byte>& fileData)
{
    if (fileData.size() < 12)
    {
        return false;
    }
    
    return !strncmp(reinterpret_cast<const char*>(&fileData[0]), "RIFF", 4) && !strncmp(reinterpret_cast<const char*>(&fileData[8]), "WEBP", 4);
}
    
} /* namespace tgon */  
