#include "PrecompiledHeader.h"

#include <ucnv_bld.h>
#include <unicode/ucnv.h>
#include <unicode/unistr.h>
#include <unicode/utypes.h>

#include "Encoding.h"

extern const UConverterSharedData* getAlgorithmicTypeFromName(const char* realName);
extern const UConverterSharedData* getAlgorithmicTypeFromCodePage(int32_t codePage);

namespace tg
{

std::unordered_map<int32_t, Encoding> Encoding::m_encodingTable;

Encoding::Encoding(const char8_t* codePageName) :
    Encoding(CreateUConverter(codePageName))
{
}

Encoding::Encoding(int32_t codePage) :
    Encoding(CreateUConverter(codePage))
{
}

Encoding::Encoding(UConverter* converter) noexcept :
    m_converter(converter),
    m_encodingName(reinterpret_cast<const char8_t*>(m_converter->sharedData->staticData->name))
{
}

Encoding::~Encoding()
{
    if (m_converter != nullptr)
    {
        ucnv_close(m_converter);
        m_converter = nullptr;
    }
}

bool Encoding::operator==(const Encoding& rhs) const noexcept
{
    return m_converter == rhs.m_converter;
}

bool Encoding::operator!=(const Encoding& rhs) const noexcept
{
    return !operator==(rhs);
}

const Encoding* Encoding::GetEncoding(int32_t codePage)
{
    constexpr auto IBM_UTF8_CCSID = 1208;
    constexpr auto IBM_UTF16LE_CCSID = 1202;
    constexpr auto IBM_UTF16BE_CCSID = 1200;
    constexpr auto IBM_UTF32LE_CCSID = 1236;
    switch (codePage)
    {
    case IBM_UTF8_CCSID:
        return &UTF8();

    case IBM_UTF16LE_CCSID:
        return &Unicode();

    case IBM_UTF16BE_CCSID:
        return &BigEndianUnicode();

    case IBM_UTF32LE_CCSID:
        return &UTF32();

    default:
        auto it = m_encodingTable.find(codePage);
        if (it == m_encodingTable.end())
        {
            it = m_encodingTable.try_emplace(it, codePage, codePage);
        }

        return &it->second;
    }
}

const Encoding* Encoding::GetEncoding(const char8_t* codePageName)
{
    const auto* converterSharedData = getAlgorithmicTypeFromName(reinterpret_cast<const char*>(codePageName));
    if (converterSharedData == nullptr)
    {
        return nullptr;
    }

    return GetEncoding(converterSharedData->staticData->codepage);
}

std::optional<std::vector<std::byte>> Encoding::Convert(const Encoding& srcEncoding, const Encoding& destEncoding, const std::byte* bytes, int32_t count)
{
    auto status = U_ZERO_ERROR;
    const icu::UnicodeString str(reinterpret_cast<const char*>(bytes), count, srcEncoding.m_converter, status);
    if (U_FAILURE(status))
    {
        return {};
    }

    const auto encodedStrByteCount = str.extract(nullptr, 0, destEncoding.m_converter, status);

    std::vector<std::byte> ret(encodedStrByteCount);
    status = U_ZERO_ERROR;
    str.extract(reinterpret_cast<char*>(&ret[0]), static_cast<int32_t>(ret.size()), destEncoding.m_converter, status);

    return ret;
}

std::optional<int32_t> Encoding::Convert(const Encoding& srcEncoding, const Encoding& destEncoding, const std::byte* srcBytes, int32_t srcByteCount, std::byte* destBytes, int32_t destByteCount)
{
    auto status = U_ZERO_ERROR;
    const icu::UnicodeString str(reinterpret_cast<const char*>(srcBytes), srcByteCount, srcEncoding.m_converter, status);
    if (U_FAILURE(status))
    {
        if (destByteCount >= destEncoding.GetMinCharByte())
        {
            memset(destBytes, 0, destEncoding.GetMinCharByte());
        }
        return {};
    }

    const auto encodedStrByteCount = str.extract(nullptr, 0, destEncoding.m_converter, status);
    if (encodedStrByteCount > destByteCount)
    {
        if (destByteCount >= destEncoding.GetMinCharByte())
        {
            memset(destBytes, 0, destEncoding.GetMinCharByte());
        }
        return {};
    }

    status = U_ZERO_ERROR;
    str.extract(reinterpret_cast<char*>(&destBytes[0]), destByteCount, destEncoding.m_converter, status);

    return encodedStrByteCount;
}

std::optional<int32_t> Encoding::Convert(const Encoding& srcEncoding, const Encoding& destEncoding, const std::span<std::byte>& srcBytes, const std::span<std::byte>& destBytes)
{
    return Convert(srcEncoding, destEncoding, &srcBytes[0], static_cast<int32_t>(srcBytes.size()), &destBytes[0], static_cast<int32_t>(destBytes.size()));
}

std::optional<std::vector<char32_t>> Encoding::GetChars(const std::byte* bytes, int32_t count) const
{
    std::vector<char32_t> ret;

    auto status = U_ZERO_ERROR;
    const auto* currentIt = reinterpret_cast<const char*>(&bytes[0]);
    const auto* endIt = reinterpret_cast<const char*>(bytes + count);
    while (currentIt < endIt)
    {
        const auto c = ucnv_getNextUChar(m_converter, &currentIt, currentIt + count, &status);
        if (U_FAILURE(status))
        {
            return {};
        }

        if (c == 0)
        {
            break;
        }

        ret.push_back(static_cast<char32_t>(c));
    }

    return std::move(ret);
}

std::optional<std::vector<char32_t>> Encoding::GetChars(const std::span<std::byte>& bytes) const
{
    return GetChars(&bytes[0], static_cast<int32_t>(bytes.size()));
}

std::optional<std::u8string> Encoding::GetString(const std::byte* bytes, int32_t count) const
{
    auto status = U_ZERO_ERROR;
    const icu::UnicodeString str(reinterpret_cast<const char*>(bytes), count, m_converter, status);
    if (U_FAILURE(status))
    {
        return {};
    }

    std::u8string ret;
    const auto encodedStrLen = str.extract(nullptr, 0, UTF8().m_converter, status);
    ret.resize(encodedStrLen);

    status = U_ZERO_ERROR;
    str.extract(reinterpret_cast<char*>(ret.data()), static_cast<int32_t>(ret.size() + 1), UTF8().m_converter, status);

    return std::move(ret);
}

std::optional<std::u8string> Encoding::GetString(const std::span<const std::byte>& bytes) const
{
    return GetString(bytes.data(), bytes.size());
}

std::span<const std::byte> Encoding::GetPreamble() const noexcept
{
    const auto* converterData = m_converter->sharedData->staticData;
    return std::span(reinterpret_cast<const std::byte*>(converterData->subChar), converterData->subCharLen);
}

std::optional<int32_t> Encoding::GetCharCount(const std::byte* bytes, int32_t count) const
{
    int32_t ret = 0;

    auto status = U_ZERO_ERROR;
    const auto* currentIt = reinterpret_cast<const char*>(&bytes[0]);
    const auto* endIt = reinterpret_cast<const char*>(bytes + count);
    while (currentIt < endIt)
    {
        const auto c = ucnv_getNextUChar(m_converter, &currentIt, currentIt + count, &status);
        if (U_FAILURE(status))
        {
            return {};
        }

        if (c == 0)
        {
            break;
        }

        ++ret;
    }

    return ret;
}

std::optional<int32_t> Encoding::GetCharCount(const std::span<std::byte>& bytes) const
{
    return GetCharCount(&bytes[0], static_cast<int32_t>(bytes.size()));
}

const std::u8string_view& Encoding::GetEncodingName() const noexcept
{
    return m_encodingName;
}

int32_t Encoding::GetCodePage() const noexcept
{
    return m_converter->sharedData->staticData->codepage;
}

size_t Encoding::GetHashCode() const noexcept
{
    return static_cast<size_t>(m_converter->sharedData->staticData->codepage);
}

const Encoding& Encoding::UTF8()
{
    static Encoding encoding(u8"UTF8");
    return encoding;
}

const Encoding& Encoding::Unicode()
{
    static Encoding encoding(u8"UTF16LE");
    return encoding;
}

const Encoding& Encoding::BigEndianUnicode()
{
    static Encoding encoding(u8"UTF16BE");
    return encoding;
}

const Encoding& Encoding::UTF32()
{
    static Encoding encoding(u8"UTF32");
    return encoding;
}

bool Encoding::IsSingleByte() const noexcept
{
    return this->GetMinCharByte() == 1 && this->GetMaxCharByte() == 1;
}

UConverter* Encoding::CreateUConverter(const char8_t* codePageName)
{
    auto status = U_ZERO_ERROR;
    auto* converter = ucnv_open(reinterpret_cast<const char*>(codePageName), &status);
    if (U_FAILURE(status))
    {
        return nullptr;
    }

    return converter;
}

UConverter* Encoding::CreateUConverter(int32_t codePage)
{
    const auto* converterSharedData = getAlgorithmicTypeFromCodePage(codePage);
    if (converterSharedData == nullptr)
    {
        return nullptr;
    }

    auto status = U_ZERO_ERROR;
    auto* converter = ucnv_open(converterSharedData->staticData->name, &status);
    if (U_FAILURE(status))
    {
        return nullptr;
    }

    return converter;
}

int32_t Encoding::GetMinCharByte() const noexcept
{
    return m_converter->sharedData->staticData->minBytesPerChar;
}

int32_t Encoding::GetMaxCharByte() const noexcept
{
    return m_converter->sharedData->staticData->maxBytesPerChar;
}

}
