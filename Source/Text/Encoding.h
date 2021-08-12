#pragma once

#include <cstdint>
#include <span>
#include <string>
#include <unordered_map>
#include <vector>

struct UConverter;

namespace tg
{

class Encoding final
{
/**@section Constructor */
public:
    explicit Encoding(const char8_t* codePageName);
    explicit Encoding(int32_t codePage);
    explicit Encoding(UConverter* converter) noexcept;

public:
    Encoding(const Encoding& rhs) = delete;
    Encoding(Encoding&& rhs) = delete;

/**@section Destructor */
public:
    ~Encoding();

/**@section Operator */
public:
    Encoding& operator=(const Encoding& rhs) = delete;
    Encoding& operator=(Encoding&& rhs) = delete;
    bool operator==(const Encoding& rhs) const noexcept;
    bool operator!=(const Encoding& rhs) const noexcept;

/**@section Method */
public:
    [[nodiscard]] static const Encoding* GetEncoding(int32_t codePage);
    [[nodiscard]] static const Encoding* GetEncoding(const char8_t* codePageName);
    [[nodiscard]] static std::optional<std::vector<std::byte>> Convert(const Encoding& srcEncoding, const Encoding& destEncoding, const std::byte* bytes, int32_t count);
    static std::optional<int32_t> Convert(const Encoding& srcEncoding, const Encoding& destEncoding, const std::byte* srcBytes, int32_t srcByteCount, std::byte* destBytes, int32_t destByteCount);
    static std::optional<int32_t> Convert(const Encoding& srcEncoding, const Encoding& destEncoding, const std::span<std::byte>& srcBytes, const std::span<std::byte>& destBytes);
    [[nodiscard]] std::optional<std::vector<char32_t>> GetChars(const std::byte* bytes, int32_t count) const;
    [[nodiscard]] std::optional<std::vector<char32_t>> GetChars(const std::span<std::byte>& bytes) const;
    [[nodiscard]] std::optional<std::u8string> GetString(const std::byte* bytes, int32_t count) const;
    [[nodiscard]] std::optional<std::u8string> GetString(const std::span<const std::byte>& bytes) const;
    [[nodiscard]] std::span<const std::byte> GetPreamble() const noexcept;
    [[nodiscard]] std::optional<int32_t> GetCharCount(const std::byte* bytes, int32_t count) const;
    [[nodiscard]] std::optional<int32_t> GetCharCount(const std::span<std::byte>& bytes) const;
    [[nodiscard]] const std::u8string_view& GetEncodingName() const noexcept;
    [[nodiscard]] int32_t GetCodePage() const noexcept;
    [[nodiscard]] size_t GetHashCode() const noexcept;
    [[nodiscard]] static const Encoding& UTF8();
    [[nodiscard]] static const Encoding& UTF32();
    [[nodiscard]] static const Encoding& Unicode();
    [[nodiscard]] static const Encoding& BigEndianUnicode();
    [[nodiscard]] bool IsSingleByte() const noexcept;

private:
    [[nodiscard]] static UConverter* CreateUConverter(const char8_t* codePageName);
    [[nodiscard]] static UConverter* CreateUConverter(int32_t codePage);
    [[nodiscard]] int32_t GetMinCharByte() const noexcept;
    [[nodiscard]] int32_t GetMaxCharByte() const noexcept;

/**@section Variable */
private:
    UConverter* m_converter = nullptr;
    std::u8string_view m_encodingName;
    static std::unordered_map<int32_t, Encoding> m_encodingTable;
};

}
