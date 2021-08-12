#include "PrecompiledHeader.h"

#include "Core/Algorithm.h"

#include "FontFactory.h"

#undef __FTERRORS_H__
#define FT_ERRORDEF( e, v, s )  {e, s},
#define FT_ERROR_START_LIST     {
#define FT_ERROR_END_LIST       {0, 0}};

namespace tg
{

const char* ConvertFTErrorToString(FT_Error error)
{
    static constexpr const struct
    {
        int errorCode;
        const char* errorMessage;
    } ftErrors[] =
    #include FT_ERRORS_H
    
    return ftErrors[error].errorMessage;
}

constexpr int32_t ConvertFTPixelModeToBits(FT_Pixel_Mode pixelMode) noexcept
{
    constexpr int32_t bytesTable[] =
    {
        1, // FT_PIXEL_MODE_MONO
        8, // FT_PIXEL_MODE_GRAY
        2, // FT_PIXEL_MODE_GRAY2
        4, // FT_PIXEL_MODE_GRAY4
        8, // FT_PIXEL_MODE_LCD
        8, // FT_PIXEL_MODE_LCD_V
        32, // FT_PIXEL_MODE_BGRA
    };

    return bytesTable[UnderlyingCast(pixelMode)];
}

FontFactory::FontFactory(FT_Library library) noexcept :
    m_library(library, [](FT_Library library)
    {
        if (library != nullptr)
        {
            FT_Done_FreeType(library);
        }
    })
{
}

std::optional<FontFactory> FontFactory::Create()
{
    FT_Library library = nullptr;
    if (FT_Init_FreeType(&library))
    {
        return {};
    }

    return FontFactory(library);
}

std::shared_ptr<std::remove_pointer_t<FT_Library>> FontFactory::GetLibrary() noexcept
{
    return m_library;
}

std::shared_ptr<const std::remove_pointer_t<FT_Library>> FontFactory::GetLibrary() const noexcept
{
    return m_library;
}

}

#undef FT_ERRORDEF
#undef FT_ERROR_START_LIST
#undef FT_ERROR_END_LIST
