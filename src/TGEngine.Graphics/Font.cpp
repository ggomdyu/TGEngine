#include "Core/MakeSharedEnabler.h"
#include "IO/File.h"

#include "Font.h"

TG_NAMESPACE_BEGIN

extern const char* ConvertFTErrorToString(FT_Error error);

Font::Font(std::shared_ptr<std::remove_pointer_t<FT_Library>>&& library, std::vector<std::byte>&& fileData) :
    _fileData(std::move(fileData)),
    _library(std::move(library))
{
}

std::shared_ptr<Font> Font::Create(std::shared_ptr<std::remove_pointer_t<FT_Library>> library, const char8_t* filePath)
{
    auto fileData = File::ReadAllBytes(filePath, ReturnVectorTag{});
    if (fileData.has_value() == false)
    {
        return {};
    }

    return Create(std::move(library), std::move(*fileData));
}

std::shared_ptr<Font> Font::Create(std::shared_ptr<std::remove_pointer_t<FT_Library>> library, std::vector<std::byte>&& fileData)
{
    return std::make_shared<MakeSharedEnabler<Font>>(std::move(library), std::move(fileData));
}

std::shared_ptr<FontFace> Font::GetFontFace(int32_t fontSize)
{
    const auto it = _fontFaces.find(fontSize);
    if (it != _fontFaces.end())
    {
        return it->second;
    }

    return _fontFaces.emplace_hint(it, fontSize, FontFace::Create(_library, std::span(_fileData), fontSize))->second;
}

std::shared_ptr<const FontFace> Font::GetFontFace(int32_t fontSize) const
{
    return const_cast<Font*>(this)->GetFontFace(fontSize);
}

}
