#include <algorithm>
#include <unicode/locid.h>
#include <unordered_map>
#include <vector>

#include "System.Text/EncodingData.h"
#include "System.Text/EncodingTable.h"

TG_NAMESPACE_BEGIN

std::vector<EncodingInfo> EncodingTable::GetEncodings()
{
    std::vector<EncodingInfo> encodings;

    // UTF-7 is not supported in cs2cpp, so adjust the vector size by -1.
    encodings.reserve(EncodingData::MappedCodePages.size() - 1);

    constexpr int32_t CodePageUTF7 = 65000;
    for (size_t i = 0; i < EncodingData::MappedCodePages.size(); ++i)
    {
        int32_t codePage = EncodingData::MappedCodePages[i];

        // Ignore the unsupported encoding.
        if (codePage == CodePageUTF7)
        {
            continue;
        }

        encodings.emplace_back(codePage, EncodingData::WebNames[i], EncodingData::EnglishNames[i]);
    }

    return encodings;
}

std::optional<int32_t> EncodingTable::GetCCSIDFromCodePage(int32_t codePage) noexcept
{
    auto predicate = [codePage](int32_t item) { return item == codePage; };

    auto it = std::find_if(EncodingData::MappedCodePages.begin(), EncodingData::MappedCodePages.end(), predicate);
    if (it == EncodingData::MappedCodePages.end())
    {
        return std::nullopt;
    }

    return static_cast<int32_t>(EncodingData::MappedCCSIDs[std::distance(EncodingData::MappedCodePages.begin(), it)]);
}

std::optional<int32_t> EncodingTable::GetCodePageFromName(std::u16string_view name)
{
    static std::unordered_map<int32_t, int32_t> nameToCodePage;
    static std::mutex mutex;

    auto lowercaseName = icu::UnicodeString(name.data());
    lowercaseName.toLower(icu::Locale::getUS());

    auto nameHash = lowercaseName.hashCode();

    [[maybe_unused]] auto lock = std::lock_guard(mutex);
    auto it = nameToCodePage.find(nameHash);
    if (it != nameToCodePage.end())
    {
        return it->second;
    }

    auto codePage = InternalGetCodePageFromName(std::u16string_view(lowercaseName.getBuffer(), lowercaseName.length()));
    if (!codePage)
    {
        return std::nullopt;
    }

    return nameToCodePage.emplace_hint(it, nameHash, *codePage)->second;
}

const CodePageDataItem& EncodingTable::GetCodePageDataItem(int32_t codePage)
{
    static auto dataItems = []()
    {
        constexpr size_t DataItemCount = EncodingData::MappedCodePages.size();
        
        std::array<CodePageDataItem, DataItemCount> dataItems{};
        for (size_t i = 0; i < DataItemCount; ++i)
        {
            dataItems[i] = CodePageDataItem(
                EncodingData::UiFamilyCodePages[i],
                EncodingData::WebNames[i],
                EncodingData::WebNames[i],
                EncodingData::WebNames[i],
                EncodingData::EnglishNames[i],
                EncodingData::Preambles[i],
                EncodingData::Flags[i]
            );
        }

        return dataItems;
    } ();

    int32_t index;
    switch (codePage)
    {
        case 1200: // utf-16
            index = 0;
            break;
        case 1201: // utf-16be
            index = 1;
            break;
        case 12000: // utf-32
            index = 2;
            break;
        case 12001: // utf-32be
            index = 3;
            break;
        case 20127: // us-ascii
            index = 4;
            break;
        case 28591: // iso-8859-1
            index = 5;
            break;
        case 65000: // utf-7
            index = 6;
            break;
        case 65001: // utf-8
            index = 7;
            break;
        default:
            static CodePageDataItem emptyDataItem;
            return emptyDataItem;
    }

    return dataItems[index];
}

std::optional<int32_t> EncodingTable::InternalGetCodePageFromName(std::u16string_view name) noexcept
{
    auto predicate = [&name](std::u16string_view item) { return item.compare(name) == 0; };

    auto it = std::find_if(EncodingData::EncodingNames.begin(), EncodingData::EncodingNames.end(), predicate);
    if (it == EncodingData::EncodingNames.end())
    {
        return std::nullopt;
    }

    return EncodingData::CodePagesByName[std::distance(EncodingData::EncodingNames.begin(), it)];
}

TG_NAMESPACE_END