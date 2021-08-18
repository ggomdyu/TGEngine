#pragma once

#include "Core/NonCopyable.h"

#include "FontFace.h"

TG_NAMESPACE_BEGIN

class Font :
    private NonCopyable
{
protected:
    Font(std::shared_ptr<std::remove_pointer_t<FT_Library>>&& library, std::vector<std::byte>&& fileData);

public:
    /**
     * @brief   Creates a instance of object.
     * @param library   The handle to the FreeType library instance.
     * @param filePath  The font file path.
     * @return  The instantiated object.
     */
    static std::shared_ptr<Font> Create(std::shared_ptr<std::remove_pointer_t<FT_Library>> library, const char8_t* filePath);

    /**
     * @brief   Creates a instance of object.
     * @param library   The handle to the FreeType library instance.
     * @param fileData  The font file data.
     * @return  The instantiated object.
     */
    static std::shared_ptr<Font> Create(std::shared_ptr<std::remove_pointer_t<FT_Library>> library, std::vector<std::byte>&& fileData);

    /**
     * @brief   Gets the font face.
     * @param fontSize  The size of font face to get.
     * @return  The font face or nullptr.
     */
    std::shared_ptr<FontFace> GetFontFace(int32_t fontSize);

    /**
     * @brief   Gets the font face.
     * @param fontSize  The size of font face to get.
     * @return  The font face or nullptr.
     */
    std::shared_ptr<const FontFace> GetFontFace(int32_t fontSize) const;

private:
    std::vector<std::byte> _fileData;
    std::shared_ptr<std::remove_pointer_t<FT_Library>> _library;
    mutable std::unordered_map<int32_t, std::shared_ptr<FontFace>> _fontFaces;
};

}
