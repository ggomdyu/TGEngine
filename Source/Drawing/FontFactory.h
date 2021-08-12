#pragma once

#include <optional>
#include <memory>
#include <ft2build.h>
#include FT_FREETYPE_H
 
#include "Core/NonCopyable.h"

namespace tg
{

class FontFactory final :
    private NonCopyable
{
/**@section Constructor */
private:
    explicit FontFactory(FT_Library library) noexcept;

/**@section Method */
public:
    /**
     * @brief   Creates a instance of object.
     * @return  The instantiated object.
     */
    [[nodiscard]] static std::optional<FontFactory> Create();

    /**
     * @brief   Gets the handle to the FreeType library instance.
     * @return  The FreeType library instance.
     */
    [[nodiscard]] std::shared_ptr<std::remove_pointer_t<FT_Library>> GetLibrary() noexcept;

    /**
     * @brief   Gets the handle to the FreeType library instance.
     * @return  The FreeType library instance.
     */
    [[nodiscard]] std::shared_ptr<const std::remove_pointer_t<FT_Library>> GetLibrary() const noexcept;

/**@section Variable */
private:
    std::shared_ptr<std::remove_pointer_t<FT_Library>> m_library;
};

}
