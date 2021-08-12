/**
 * @file    PngImageProcessor.h
 * @author  ggomdyu
 * @since   01/20/2018
 */

#pragma once
#include <png.h>

#include "BaseImageProcessor.h"

namespace tgon
{

class PngImageProcessor :
    public BaseImageProcessor<PngImageProcessor>
{
/* @section Constructor */
public:
    using BaseImageProcessor<PngImageProcessor>::BaseImageProcessor;

/* @section Method */
public:
    /* @brief   Verifies the file format is exact. */
    static bool VerifyFormat(const uint8_t* fileData, size_t fileDataBytes);

    /* @brief   Decodes the file to the image. */
    bool Import(const uint8_t* fileData, size_t fileDataBytes);
};

inline bool PngImageProcessor::Import(const uint8_t* fileData, size_t fileDataBytes)
{
    if (VerifyFormat(fileData, fileDataBytes) == false)
    {
        return false;
    }

    // Allocate and initialize a png_struct structure.
    // You can pass pointers to error handling functions, and a pointer to a data struct 
    // for use by the error functions, if necessary (the pointer and functions can be NULL if the default error handlers are to be used).
    png_structp pngStruct = png_create_read_struct(PNG_LIBPNG_VER_STRING, nullptr, nullptr, nullptr);
    if (pngStruct == nullptr)
    {
        return false;
    }

    png_infop pngInfo = png_create_info_struct(pngStruct);
    if (pngInfo == nullptr)
    {
        png_destroy_read_struct(&pngStruct, nullptr, nullptr);
        return false;
    }

    // We can change the method of I/O. libPNG supplies callbacks that you can set through the function png_set_read_fn() at run time.
    // We will use it for reading PNG Images from memory.
    {
        struct ImageSource
        {
            png_size_t size;
            const uint8_t* data;
            png_size_t offset;
        } imageSource{static_cast<png_size_t>(fileDataBytes), reinterpret_cast<const uint8_t*>(fileData), 0};

        png_set_read_fn(pngStruct, &imageSource, [](png_structp pngStruct, png_bytep data, png_size_t dataLen)
        {
            ImageSource* imageSource = reinterpret_cast<ImageSource*>(png_get_io_ptr(pngStruct));

            if ((imageSource->offset + dataLen) <= imageSource->size)
            {
                memcpy(data, imageSource->data + imageSource->offset, dataLen);
                imageSource->offset += dataLen;
            }
        });
    }

    // Set error handling with setjmp.
    // Unless if you set up your own error handlers in the png_create_read_struct earlier.
    if (setjmp(png_jmpbuf(pngStruct)))
    {
        png_destroy_read_struct(&pngStruct, &pngInfo, nullptr);
        return false;
    }

    // Read the PNG file information.
    png_read_info(pngStruct, pngInfo);

    m_width = png_get_image_width(pngStruct, pngInfo);
    m_height = png_get_image_height(pngStruct, pngInfo);
    m_colorDepth = png_get_bit_depth(pngStruct, pngInfo);
    m_channels = png_get_channels(pngStruct, pngInfo);
    m_pixelFormat = PixelFormat::RGBA8888;

    png_uint_32 colorType = png_get_color_type(pngStruct, pngInfo);
    if (colorType == PNG_COLOR_TYPE_PALETTE)
    {
        png_set_palette_to_rgb(pngStruct);
    }

    else if (colorType == PNG_COLOR_TYPE_GRAY && m_colorDepth < 8)
    {
        png_set_expand_gray_1_2_4_to_8(pngStruct);
    }

    // Expand tRNS chunk data to full alpha channel.
    if (png_get_valid(pngStruct, pngInfo, PNG_INFO_tRNS))
    {
        png_set_tRNS_to_alpha(pngStruct);
    }

    if (m_colorDepth == 16)
    {
        png_set_strip_16(pngStruct);
    }

    // If color type has no alpha channel, then fill it with 0xff.
    if (colorType == PNG_COLOR_TYPE_RGB || colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_PALETTE)
    {
        png_set_filler(pngStruct, 0xff, PNG_FILLER_AFTER);
    }
    // If the image is grayscale, then expand it to 24-bit RGB.
    if (colorType == PNG_COLOR_TYPE_GRAY || colorType == PNG_COLOR_TYPE_GRAY_ALPHA)
    {
        png_set_gray_to_rgb(pngStruct);
    }

    png_read_update_info(pngStruct, pngInfo);

    // Start reading the image.
    {
        png_size_t rowBytes = png_get_rowbytes(pngStruct, pngInfo);
        size_t imageDataBytes = sizeof(png_byte*) * (m_height) * rowBytes;
        m_imageData = std::make_unique<uint8_t[]>(imageDataBytes);

        thread_local static std::vector<png_byte*> rowPointer;
        rowPointer.resize(m_height);

        for (size_t i = 0; i < m_height; ++i)
        {
            rowPointer[i] = m_imageData.get() + i * rowBytes;
        }

        png_read_image(pngStruct, rowPointer.data());
        png_read_end(pngStruct, nullptr);
    }

    png_destroy_read_struct(&pngStruct, &pngInfo, nullptr);
    return true;
}

inline bool PngImageProcessor::VerifyFormat(const uint8_t* fileData, size_t fileDataBytes)
{
    if (fileDataBytes < 8)
    {
        return false;
    }

    bool isPNGFormat = png_sig_cmp(fileData, 0, 8) == 0;
    return isPNGFormat;
}

} /* namespace tgon */  
