#pragma once

#include <optional>
#include <memory>
#include <ft2build.h>
#include FT_FREETYPE_H
 
#include "Core/NonCopyable.h"

TG_NAMESPACE_BEGIN

class FontFactory final :
    private NonCopyable
{
private:
    explicit FontFactory(FT_Library library) noexcept;

public:
    static std::optional<FontFactory> Create();
    std::shared_ptr<std::remove_pointer_t<FT_Library>> GetLibrary() noexcept;
    std::shared_ptr<const std::remove_pointer_t<FT_Library>> GetLibrary() const noexcept;

private:
    std::shared_ptr<std::remove_pointer_t<FT_Library>> _library;
};

TG_NAMESPACE_END