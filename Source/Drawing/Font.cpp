#include "PrecompiledHeader.h"

#include "Core/MakeSharedEnabler.h"
#include "IO/File.h"

#include "Font.h"

namespace tg
{

extern const char* ConvertFTErrorToString(FT_Error error);

Font::Font(std::shared_ptr<std::remove_pointer_t<FT_Library>>&& library, std::vector<std::byte>&& fileData) :
    m_fileData(std::move(fileData)),
    m_library(std::move(library))
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
    const auto it = m_fontFaces.find(fontSize);
    if (it != m_fontFaces.end())
    {
        return it->second;
    }

    return m_fontFaces.emplace_hint(it, fontSize, FontFace::Create(m_library, std::span(m_fileData), fontSize))->second;
}

std::shared_ptr<const FontFace> Font::GetFontFace(int32_t fontSize) const
{
    return const_cast<Font*>(this)->GetFontFace(fontSize);
}

}
